#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "elf/elf.hpp"
#include "hijacker/hijacker.hpp"
#include "util.hpp"

extern int runKlogger(void *unused);
extern int runElfServer(void *unused);
extern int runCommandProcessor(void *unused);

static constexpr int ELF_PORT = 9027;

extern "C" ssize_t _read(int, void *, size_t);
extern void printBacktrace();

class Socket {
	int fd = -1;

	void close() {
		if (fd != -1) {
			::close(fd);
			fd = -1;
		}
}

	public:
		Socket() = default;
		Socket(int fd) : fd(fd) {}
		Socket(const Socket&) = delete;
		Socket(Socket &&rhs) noexcept : fd(rhs.fd) { rhs.fd = -1; }
		Socket &operator=(int fd) {
			close();
			this->fd = fd;
			return *this;
		}
		Socket &operator=(const Socket &rhs) = delete;
		Socket &operator=(Socket &&rhs) noexcept {
			close();
			fd = rhs.fd;
			rhs.fd = -1;
			return *this;
		}
		~Socket() {
			close();
		}

		int getFd() const { return fd; }
		explicit operator bool() const { return fd != -1; }
		bool read(uint8_t *buf, size_t size) const {
			while (size > 0) {
				auto read = _read(fd, buf, size);
				if (read == -1) {
					return false;
				}
				size -= read;
				buf += read;
			}
			return true;
		}
};

bool runElf(Hijacker *hijacker, uint16_t port) {
	UniquePtr<uint8_t[]> buf = nullptr;
	{
		Socket sock = socket(AF_INET, SOCK_STREAM, 0);

		if (!sock) {
			__builtin_printf("socket: %s", strerror(errno));
			return false;
		}

		int value = 1;
		if (setsockopt(sock.getFd(), SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) < 0) {
			__builtin_printf("setsockopt: %s", strerror(errno));
			return false;
		}

		//__builtin_memset(&server_addr, 0, sizeof(server_addr));
		struct sockaddr_in server_addr{0, AF_INET, htons(port), {}, {}};

		if (bind(sock.getFd(), reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) != 0) {
			__builtin_printf("bind: %s", strerror(errno));
			return false;
		}

		if (listen(sock.getFd(), 1) != 0) {
			__builtin_printf("listen: %s", strerror(errno));
			return false;
		}

		struct sockaddr client_addr{};
		socklen_t addr_len = sizeof(client_addr);
		Socket conn = accept(sock.getFd(), &client_addr, &addr_len);
		if (!conn) {
			__builtin_printf("accept: %s", strerror(errno));
			return false;
		}

		ssize_t size = 0;
		if (_read(conn.getFd(), &size, sizeof(size)) == -1) {
			__builtin_printf("read size: %s", strerror(errno));
			return false;
		}


		buf = {new uint8_t[size]};

		if (!conn.read(buf.get(), size)) {
			return false;
		}
	}

	Elf elf{hijacker, buf.release()};

	if (!elf.launch()) {
		return false;
	}

	return true;
}

class FileDescriptor {
	int fd = -1;

	void close() {
		if (fd != -1) {
			::close(fd);
			fd = -1;
		}
}

	public:
		FileDescriptor() = default;
		FileDescriptor(int fd) : fd(fd) {}
		FileDescriptor(const FileDescriptor&) = delete;
		FileDescriptor(FileDescriptor &&rhs) noexcept : fd(rhs.fd) { rhs.fd = -1; }
		FileDescriptor &operator=(int fd) {
			close();
			this->fd = fd;
			return *this;
		}
		FileDescriptor &operator=(const FileDescriptor &rhs) = delete;
		FileDescriptor &operator=(FileDescriptor &&rhs) noexcept {
			close();
			fd = rhs.fd;
			rhs.fd = -1;
			return *this;
		}
		~FileDescriptor() {
			close();
		}

		operator int() const { return fd; }
		explicit operator bool() const { return fd != -1; }
		bool read(uint8_t *buf, size_t size) const {
			while (size > 0) {
				auto read = _read(fd, buf, size);
				if (read == -1) {
					__builtin_printf("read failed error %s\n", strerror(errno));
					printBacktrace();
					return false;
				}
				size -= read;
				buf += read;
			}
			return true;
		}

		void release() { fd = -1; }
};

extern "C" void *start_ftp(void*);

int main() {
	pthread_t trd = nullptr;
	pthread_create(&trd, NULL, start_ftp, nullptr);
	while (true) {
		auto hijacker = Hijacker::getHijacker(getpid());
		runElf(hijacker.get(), ELF_PORT);
	}
	pthread_join(trd, nullptr);
}

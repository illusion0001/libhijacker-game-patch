#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
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

static constexpr int LOGGER_PORT = 9021;

static constexpr int STDOUT = 1;
static constexpr int STDERR = 2;

static constexpr int ELF_PORT = 9027;

extern "C" ssize_t _read(int, void *, size_t);
extern void printBacktrace();

class Socket {
	int fd = -1;

	void close() {
		if (fd != -1) {
			__builtin_printf("closing socket %d\n", fd);
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
					__builtin_printf("read failed error %s\n", strerror(errno));
					printBacktrace();
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
		puts("entered runElf");
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

		puts("binding socket");
		if (bind(sock.getFd(), reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) != 0) {
			__builtin_printf("bind: %s", strerror(errno));
			return false;
		}

		puts("listening");
		if (listen(sock.getFd(), 1) != 0) {
			__builtin_printf("listen: %s", strerror(errno));
			return false;
		}

		struct sockaddr client_addr{};
		socklen_t addr_len = sizeof(client_addr);
		__builtin_printf("waiting for connection to load elf on port %d\n", port);
		Socket conn = accept(sock.getFd(), &client_addr, &addr_len);
		if (!conn) {
			__builtin_printf("accept: %s", strerror(errno));
			return false;
		}

		__builtin_printf("connection accepted, connfd: %d\n", conn.getFd());

		ssize_t size = 0;
		if (_read(conn.getFd(), &size, sizeof(size)) == -1) {
			__builtin_printf("read size: %s", strerror(errno));
			return false;
		}

		__builtin_printf("elf size: %lld\n", size);

		buf = {new uint8_t[size]};

		if (!conn.read(buf.get(), size)) {
			return false;
		}
	}

	Elf elf{hijacker, buf.release()};

	if (!elf.launch()) {
		puts("launch failed");
		return false;
	}

	puts("launch succeeded");
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

static int initStdout() {
	{
		FileDescriptor sock = socket(AF_INET, SOCK_STREAM, 0);

		if (!sock) {
			return -1;
		}

		int value = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) < 0) {
			return -1;
		}

		struct sockaddr_in server_addr{0, AF_INET, htons(LOGGER_PORT), {}, {}};

		if (bind(sock, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) != 0) {
			return -1;
		}

		if (listen(sock, 1) != 0) {
			return -1;
		}

		struct sockaddr client_addr{};
		socklen_t addr_len = sizeof(client_addr);
		int conn = accept(sock, &client_addr, &addr_len);
		if (conn != -1) {
			return conn;
		}
	}
	return -1;
}

class Stdout {
	int fd = -1;

	public:
		explicit Stdout() : fd(initStdout()) {
			if (fd != -1) {
				dup2(fd, STDOUT);
				dup2(fd, STDERR);
				close(fd);
			}
		}
		Stdout(const Stdout&) = delete;
		Stdout &operator=(const Stdout&) = delete;
		Stdout(Stdout&&) = delete;
		Stdout &operator=(Stdout&&) = delete;
		~Stdout() {
			if (fd != -1) {
				close(STDOUT);
				close(STDERR);
			}
		}
};

int main() {
	Stdout dummy{};
	puts("shim entered");
	while (true) {
		auto hijacker = Hijacker::getHijacker(getpid());
		runElf(hijacker.get(), ELF_PORT);
	}
}

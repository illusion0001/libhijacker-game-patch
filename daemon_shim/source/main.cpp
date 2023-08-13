#include <arpa/inet.h>
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
static constexpr int KERNELRW_PORT = 9030;
static constexpr int STUPID_C_ERROR_VALUE = -1;

extern "C" ssize_t _read(int, void *, size_t);
extern void printBacktrace();

enum SystemEventType : uint32_t {
	RESUME_EVENT = 0x10000000
};

struct SystemEvent {
	static constexpr size_t UNKNOWN_LENGTH = 0x2000;
	SystemEventType type;
	uint8_t unknown[UNKNOWN_LENGTH];
};

struct SystemStatus {
	static constexpr size_t UNKNOWN_LENGTH = 0xfc;
	uint32_t numEvents;
	uint8_t unknown[UNKNOWN_LENGTH];
};

extern "C" uint32_t sceSystemServiceReceiveEvent(SystemEvent *event);
extern "C" uint32_t sceSystemServiceGetStatus(SystemStatus *status);

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

static void *kernelRWHandler(void *unused) noexcept {
	(void)unused;
	FileDescriptor server = socket(AF_INET, SOCK_STREAM, 0);
	if (server == STUPID_C_ERROR_VALUE) {
		perror("kernelRWHandler socket");
		return nullptr;
	}
	int value = 1;
	if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) == STUPID_C_ERROR_VALUE) {
		perror("kernelRWHandler setsockopt");
		return nullptr;
	}

	struct sockaddr_in server_addr{0, AF_INET, htons(KERNELRW_PORT), {inet_addr("127.0.0.1")}, {}};

	if (bind(server, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == STUPID_C_ERROR_VALUE) {
		perror("kernelRWHandler bind");
		return nullptr;
	}

	if (listen(server, 1) == STUPID_C_ERROR_VALUE) {
		perror("kernelRWHandler listen");
		return nullptr;
	}

	while (true) {
		struct sockaddr client_addr{};
		socklen_t addr_len = sizeof(client_addr);
		FileDescriptor conn = ::accept(server, &client_addr, &addr_len);
		if (conn == STUPID_C_ERROR_VALUE) {
			perror("kernelRWHandler accept");
			continue;
		}

		int pid = 0;
		int sockets[2]{};
		ssize_t n = read(conn, &pid, sizeof(pid));
		if (n == STUPID_C_ERROR_VALUE) {
			perror("kernelRWHandler read");
			continue;
		}
		if (n != sizeof(pid)) {
			puts("kernelRWHandler didn't read enough data for pid");
			continue;
		}
		n = read(conn, sockets, sizeof(sockets));
		if (n == STUPID_C_ERROR_VALUE) {
			perror("kernelRWHandler read");
			continue;
		}
		if (n != sizeof(sockets)) {
			puts("kernelRWHandler didn't read enough data for sockets");
			continue;
		}
		if (!createReadWriteSockets(pid, sockets)) {
			printf("failed to create kernelrw sockets for pid %d\n", pid);
			continue;
		}
		n = write(conn, &kernel_base, sizeof(kernel_base));
		if (n == STUPID_C_ERROR_VALUE) {
			perror("kernelRWHandler write");
			continue;
		}
		if (n != sizeof(sockets)) {
			puts("kernelRWHandler didn't write enough data for response");
			continue;
		}
	}
	return nullptr;
}

int main() {
	static SystemEvent event{};
	static SystemStatus status{};
	while (true) {
		pthread_t ftp = nullptr;
		pthread_t krw = nullptr;
		pthread_create(&ftp, nullptr, start_ftp, nullptr);
		pthread_create(&krw, nullptr, kernelRWHandler, nullptr);
		while (true) {
			auto hijacker = Hijacker::getHijacker(getpid());
			runElf(hijacker.get(), ELF_PORT);
		}
		pthread_join(ftp, nullptr);
		pthread_join(krw, nullptr);
		uint32_t err = sceSystemServiceGetStatus(&status);
		const uint32_t numEvents = status.numEvents;
		if (err != 0 || status.numEvents == 0) {
			break;
		}
		bool isResuming = false;
		for (uint32_t i = 0; i < numEvents && !isResuming; i++) {
			if (sceSystemServiceReceiveEvent(&event) != 0) {
				return -1;
			}
			isResuming = event.type == SystemEventType::RESUME_EVENT;
		}
		if (!isResuming) {
			break;
		}
	}
	return 0;
}

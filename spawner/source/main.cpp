#include "backtrace.hpp"
#include "dbg.hpp"
#include "elf/elf.hpp"
#include "kernel.hpp"
#include "hijacker.hpp"
#include "util.hpp"
#include <unistd.h>


extern "C" {
	#include <stdarg.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
	#include <errno.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <strings.h>
	#include <signal.h>
	#include <fcntl.h>
	ssize_t _read(int, void *, size_t);
}

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
		Socket(Socket &&rhs) : fd(rhs.fd) { rhs.fd = -1; }
		Socket &operator=(int fd) {
			close();
			this->fd = fd;
			return *this;
		}
		Socket &operator=(const Socket &rhs) = delete;
		Socket &operator=(Socket &&rhs) {
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
					return false;
				}
				size -= read;
				buf += read;
			}
			return true;
		}
};

bool runElf(Hijacker *hijacker, uint16_t port) {
	socklen_t addr_len;
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

		if (bind(sock.getFd(), (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
			__builtin_printf("bind: %s", strerror(errno));
			return false;
		}

		if (listen(sock.getFd(), 1) != 0) {
			__builtin_printf("listen: %s", strerror(errno));
			return false;
		}

		struct sockaddr client_addr{};
		addr_len = sizeof(client_addr);
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

		__builtin_printf("elf size: %lld\n", (long long)size);

		buf = new uint8_t[size];

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

[[maybe_unused]] static void __attribute__((naked, noinline)) clearFramePointer() {
	// this clears the frame pointer so we stop the backtrace at the start of our code
	__asm__ volatile(
		"mov $0, %rbp\n"
		"ret\n"
	);
}

int main() {
	//clearFramePointer();
	puts("main entered");
	auto spawner = Spawner::getSpawner("SceRedisServer");
	if (spawner == nullptr) {
		puts("failed to get spawner for SceRedisServer");
		return -1;
	}
	puts("jailbreaking original SceRedisServer process");
	spawner->getHijacker()->jailbreak();

	puts("spawning new SceRedisServer process");
	auto redis = spawner->spawn();

	if (redis == nullptr) {
		puts("failed to spawn new redis server process");
		return -1;
	}
	puts("getting saved stack pointer");
	while (redis->getSavedRsp() == 0) {
		usleep(1);
	}
	puts("setting process name");
	redis->getProc()->setName("HomebrewDaemon"_sv);
	__builtin_printf("new process %s pid %d\n", redis->getProc()->getSelfInfo()->name, redis->getPid());
	puts("jailbreaking new process");
	redis->jailbreak();

	// listen on a port for now. in the future embed the daemon and load directly

	if (runElf(redis.get(), 9027)) {
		__builtin_printf("process name %s pid %d\n", redis->getProc()->getSelfInfo()->name, redis->getPid());
	}

	return 0;
}

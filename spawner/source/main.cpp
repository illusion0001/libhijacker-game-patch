#include "backtrace.hpp"
#include "dbg.hpp"
#include "dbg/dbg.hpp"
#include "elf/elf.hpp"
#include "kernel.hpp"
#include "hijacker.hpp"
#include "kernel/kernel.hpp"
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

/*
static constexpr int LOGGER_PORT = 9021;
//static constexpr int ELF_PORT = 9027;

static constexpr int STDOUT = 1;
static constexpr int STDERR = 2;
*/

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
		FileDescriptor(FileDescriptor &&rhs) : fd(rhs.fd) { rhs.fd = -1; }
		FileDescriptor &operator=(int fd) {
			close();
			this->fd = fd;
			return *this;
		}
		FileDescriptor &operator=(const FileDescriptor &rhs) = delete;
		FileDescriptor &operator=(FileDescriptor &&rhs) {
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
					return false;
				}
				size -= read;
				buf += read;
			}
			return true;
		}

		void release() { fd = -1; }
};

extern "C" unsigned int daemon_size;
extern "C" uint8_t daemon_start[];

bool runElf(Hijacker *hijacker) {

	// we need a writeable copy
	UniquePtr<uint8_t[]> buf = new uint8_t[daemon_size];
	__builtin_memcpy(buf.get(), daemon_start, daemon_size);

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

/*
static bool initStdout() {
	socklen_t addr_len;
	{
		FileDescriptor sock = socket(AF_INET, SOCK_STREAM, 0);

		if (!sock) {
			return false;
		}

		int value = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) < 0) {
			return false;
		}

		struct sockaddr_in server_addr{0, AF_INET, htons(LOGGER_PORT), {}, {}};

		if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
			return false;
		}

		if (listen(sock, 1) != 0) {
			return false;
		}

		struct sockaddr client_addr{};
		addr_len = sizeof(client_addr);
		int conn = accept(sock, &client_addr, &addr_len);
		if (conn == -1) {
			return false;
		}
		dup2(conn, STDOUT);
		dup2(conn, STDERR);
	}
	return true;
}
*/

int main() {
	//initStdout();
	//clearFramePointer();
	puts("main entered");
	if (dbg::getProcesses().length() == 0) {
		puts("This kernel version is not yet supported :(");
		return -1;
	}

	auto hijacker = Hijacker::getHijacker("SceRedisServer"_sv);
	if (hijacker == nullptr) {
		puts("failed to get hijacker for SceRedisServer");
		return -1;
	}

	hijacker->jailbreak();

	Spawner spawner{*hijacker};

	puts("spawning new SceRedisServer process");
	auto redis = spawner.bootstrap(*hijacker);

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

	if (runElf(redis.get())) {
		__builtin_printf("process name %s pid %d\n", redis->getProc()->getSelfInfo()->name, redis->getPid());
	} else {
		// TODO kill spawned process on error
		if (Hijacker::getHijacker("SceRedisServer"_sv) == nullptr) {
			puts("SceRedisServer died, restart required");
		}
	}

	return 0;
}

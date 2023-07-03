#include "backtrace.hpp"
#include "dbg.hpp"
#include "dbg/dbg.hpp"
#include "elf/elf.hpp"
#include "kernel.hpp"
#include "hijacker.hpp"
#include "kernel/kernel.hpp"
#include "util.hpp"
#include <elf.h>
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


static constexpr int LOGGER_PORT = 9021;
//static constexpr int ELF_PORT = 9027;

static constexpr int STDOUT = 1;
static constexpr int STDERR = 2;


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

	Elf elf{hijacker, daemon_start};

	while (true) {
		// forcefully trying like this can cause a panic on shutdown after x amount of failed attempts
		if (elf.launch()) {
			puts("launch succeeded");
			return true;
		}
		puts("launch failed");
		if (Hijacker::getHijacker("SceRedisServer"_sv) == nullptr) {
			puts("SceRedisServer died, restart required");
			return false;
		}
	}

}

[[maybe_unused]] static void __attribute__((naked, noinline)) clearFramePointer() {
	// this clears the frame pointer so we stop the backtrace at the start of our code
	__asm__ volatile(
		"mov $0, %rbp\n"
		"ret\n"
	);
}

static int initStdout() {
	socklen_t addr_len;
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

		if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
			return -1;
		}

		if (listen(sock, 1) != 0) {
			return -1;
		}

		struct sockaddr client_addr{};
		addr_len = sizeof(client_addr);
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
		~Stdout() {
			if (fd != -1) {
				close(STDOUT);
				close(STDERR);
			}
		}
};


extern "C" const uintptr_t __text_start[] __attribute__((weak));
extern "C" const Elf64_Rela __rela_start[] __attribute__((weak));
extern "C" const Elf64_Rela __rela_stop[] __attribute__((weak));

static bool hasUnprocessedRelocations() {
	if ((uintptr_t)__rela_start != (uintptr_t)__rela_stop) {
		const uintptr_t imagebase = (uintptr_t)__text_start;
		const uintptr_t *ptr = (uintptr_t *)(__rela_start[0].r_offset + imagebase);
		return *ptr == 0;
	}
	return false;
}

static bool processRelocations() {
	const uintptr_t imagebase = (uintptr_t)__text_start;
	for (const Elf64_Rela __restrict *it = __rela_start; it != __rela_stop; it++) {
		if (ELF64_R_TYPE(it->r_info) != R_X86_64_RELATIVE) [[unlikely]] {
			printf("unexpected relocation type %d\n", ELF64_R_TYPE(it->r_info));
			return false;
		}
		*(uintptr_t*)(imagebase + it->r_offset) = imagebase + it->r_addend;
	}
	return true;
}

extern "C" int main() {
	Stdout dummy{};
	///clearFramePointer();
	puts("main entered");
	if (hasUnprocessedRelocations()) {
		puts("fixing unprocessed relocations for spawner.elf");
		processRelocations();
	}
	if (dbg::getProcesses().length() == 0) {
		puts("This kernel version is not yet supported :(");
		return -1;
	}

	if (Hijacker::getHijacker("HomebrewDaemon"_sv) != nullptr) {
		puts("HomebrewDaemon is already running");
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
	}

	return 0;
}

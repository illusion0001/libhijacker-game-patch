#include <errno.h>
#include <ps5/payload_main.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stddef.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "fd.hpp"
#include "hijacker/hijacker.hpp"
#include "hijacker/spawner.hpp"
#include "thread.hpp"
#include "util.hpp"

static constexpr int ELF_PORT = 9027;
static constexpr int STUPID_C_ERROR_VALUE = -1;
static constexpr uint32_t ELF_MAGIC = 0x464C457F;

static const char *EBOOT_PATH = "/app0/eboot.bin";
static const char *SPAWN_ARGS[] = {NULL};

struct AppStatus {
	unsigned int id;
	unsigned char pad[28];
};

struct LocalProcessArgs {
	int fds[2];
	int a;
	int b;
	unsigned long long pad[2];
};

extern "C" int sceSystemServiceGetAppStatus(AppStatus *status);
extern "C" int sceSystemServiceAddLocalProcess(unsigned int id, const char *path, const char **argv, LocalProcessArgs *args);

enum ProcessType : uint8_t {
	INVALID,
	DAEMON,
	GAME
};

enum class ResponseType : int8_t {
	OK,
	ERROR = -1
};

static UniquePtr<Hijacker> spawn(const uint8_t *elf) {
	Spawner spawner{};

	// start the new process
	AppStatus status;
	int res = sceSystemServiceGetAppStatus(&status);
	if (res < 0) [[unlikely]] {
		printf("sceSystemServiceGetAppStatus failed 0x%08x\n", (unsigned int)res);
		return nullptr;
	}

	LocalProcessArgs param{{0, 0}, 1, -1, {0, 0}};

	// TODO: find out if the socketpair can be closed or not
	if (socketpair(1, 1, 0, (int *) param.fds) == STUPID_C_ERROR_VALUE) {
		int err = errno;
		printf("socketpair failed %d %s\n", err, strerror(err));
		return nullptr;
	}

	// afaik this doesn't block
	res = sceSystemServiceAddLocalProcess(status.id, EBOOT_PATH, SPAWN_ARGS, &param);
	if (res < 0) [[unlikely]] {
		printf("sceSystemServiceAddLocalProcess failed 0x%08x\n", (unsigned int)res);
		return nullptr;
	}

	// race it
	return spawner.spawn();
}

static void run(int s) {
	FileDescriptor sock{s};
	ResponseType response = ResponseType::ERROR;
	ProcessType type = INVALID;
	if (read(sock, &type, sizeof(type)) == STUPID_C_ERROR_VALUE) {
		int err = errno;
		printf("read failed %d %s\n", err, strerror(err));
		return;
	}
	switch (type) {
		case DAEMON:
			break;
		case GAME:
			puts("Game process not yet supported");
			[[fallthrough]];
		default:
			if (write(sock, &response, sizeof(response)) == STUPID_C_ERROR_VALUE) {
				int err = errno;
				printf("write failed %d %s\n", err, strerror(err));
			}
			return;
	}

	char name[32];
	if (read(sock, name, sizeof(name)) == STUPID_C_ERROR_VALUE) {
		int err = errno;
		printf("read failed %d %s\n", err, strerror(err));
		return;
	}

	size_t elfSize = 0;
	if (read(sock, &elfSize, sizeof(elfSize)) == STUPID_C_ERROR_VALUE) {
		int err = errno;
		printf("read failed %d %s\n", err, strerror(err));
		return;
	}

	UniquePtr<uint8_t[]> buf = new uint8_t[elfSize];
	if (read(sock, buf.get(), sizeof(elfSize)) == STUPID_C_ERROR_VALUE) {
		int err = errno;
		printf("read failed %d %s\n", err, strerror(err));
		return;
	}
	if (*(uint32_t *)buf.get() != ELF_MAGIC) {
		puts("invalid elf");
		if (write(sock, &response, sizeof(response)) == STUPID_C_ERROR_VALUE) {
			int err = errno;
			printf("write failed %d %s\n", err, strerror(err));
		}
		return;
	}

	auto hijacker = spawn(buf.get());
	if (hijacker == nullptr) {
		if (write(sock, &response, sizeof(response)) == STUPID_C_ERROR_VALUE) {
			int err = errno;
			printf("write failed %d %s\n", err, strerror(err));
		}
		return;
	}

	hijacker->getProc()->setName(name);

	response = ResponseType::OK;

	if (write(sock, &response, sizeof(response)) == STUPID_C_ERROR_VALUE) {
		int err = errno;
		printf("write failed %d %s\n", err, strerror(err));
	}
}

int runElfServer(void *unused) {
	FileDescriptor sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		return 0;
	}

	int value = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) < 0) {
		return 0;
	}

	struct sockaddr_in server_addr{0, AF_INET, htons(ELF_PORT), {}, {}};

	if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
		return 0;
	}

	if (listen(sock, 1) != 0) {
		return 0;
	}

	struct sockaddr client_addr{};
	socklen_t addr_len = sizeof(client_addr);
	while (true) {
		int fd = accept(sock, &client_addr, &addr_len);
		if (fd != -1) {
			run(fd);
		}
	}
}

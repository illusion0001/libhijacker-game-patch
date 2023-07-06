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
extern "C" uint64_t sceSystemServiceAddLocalProcess(unsigned int id, const char *path, const char **argv, LocalProcessArgs *args);

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
	uint64_t ans = sceSystemServiceAddLocalProcess(status.id, EBOOT_PATH, SPAWN_ARGS, &param);
	if (ans != 0) [[unlikely]] {
		printf("sceSystemServiceAddLocalProcess failed 0x%08llx\n", (unsigned long long) ans);
		return nullptr;
	}

	// race it
	return spawner.spawn();
}

static void run(int s) {
	FileDescriptor sock{s};
	ResponseType response = ResponseType::ERROR;
	ProcessType type = INVALID;
	if (!sock.read(&type, sizeof(type))) {
		return;
	}
	switch (type) {
		case DAEMON:
			break;
		case GAME:
			sock.write(&response, sizeof(response));
			sock.println("Game process not yet supported");
			break;
		default:
			sock.write(&response, sizeof(response));
			sock.println("Unexpected process type");
			return;
	}

	char name[32];
	if (!sock.read(name, sizeof(name))) {
		return;
	}

	size_t elfSize = 0;
	if (!sock.read(&elfSize, sizeof(elfSize))) {
		return;
	}

	UniquePtr<uint8_t[]> buf = new uint8_t[elfSize];
	if (!sock.read(buf.get(), sizeof(elfSize))) {
		return;
	}
	if (*(uint32_t *)buf.get() != ELF_MAGIC) {
		sock.write(&response, sizeof(response));
		sock.println("invalid elf");
		return;
	}

	int out = dup(1);
	int err = dup(1);
	dup2(sock, 1);
	dup2(sock, 2);
	auto hijacker = spawn(buf.get());
	dup2(out, 1);
	dup2(err, 2);
	if (hijacker == nullptr) {
		sock.write(&response, sizeof(response));
		sock.println("spawn failed");
		return;
	}

	hijacker->getProc()->setName(name);

	response = ResponseType::OK;

	sock.write(&response, sizeof(response));
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

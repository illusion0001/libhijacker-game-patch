#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "dbg/dbg.hpp"
#include "fd.hpp"
#include "launcher.hpp"
#include "thread.hpp"

static constexpr int COMMAND_PORT = 9028;

enum Command : int8_t {
	INVALID_CMD = -1,
	ACTIVE_CMD = 0,
	LAUNCH_CMD,
	PROCLIST_CMD
};

static void processCommand(const FileDescriptor &fd);
bool launchApp(const char *titleId, bool block=false);

int runCommandProcessor(void *unused) {
	// for now this is solely for checking if we are running
	FileDescriptor sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		return 0;
	}

	int value = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) < 0) {
		return 0;
	}

	struct sockaddr_in server_addr{0, AF_INET, htons(COMMAND_PORT), {}, {}};

	if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
		return 0;
	}

	if (listen(sock, 1) != 0) {
		return 0;
	}

	struct sockaddr client_addr{};
	socklen_t addr_len = sizeof(client_addr);
	while (true) {
		FileDescriptor fd = accept(sock, &client_addr, &addr_len);
		if (fd != -1) {
			processCommand(fd);
		}
	}
	return 0;
}

static void replyError(const FileDescriptor &fd) {
	const Command cmd = INVALID_CMD;
	fd.write(&cmd, sizeof(cmd));
}

static void replyOk(const FileDescriptor &fd) {
	const Command cmd = ACTIVE_CMD;
	fd.write(&cmd, sizeof(cmd));
}

static void processCommand(const FileDescriptor &fd) {
	Command cmd = INVALID_CMD;
	if (!fd.read(&cmd, sizeof(cmd))) {
		return;
	}

	switch (cmd) {
		case ACTIVE_CMD:
			replyOk(fd);
			break;
		case LAUNCH_CMD:
			// NPXS40000
			char appId[10];
			if (!fd.read(appId, sizeof(appId) - 1)) {
				replyError(fd);
				break;
			}
			appId[sizeof(appId) - 1] = '\0';
			if (launchApp(appId)) {
				replyOk(fd);
			} else {
				replyError(fd);
			}
			break;
		case PROCLIST_CMD:
			for (auto p : dbg::getProcesses()) {
				printf("%s: %d %s\n", p.name().c_str(), p.pid(), p.path().c_str());
			}
			break;
		case INVALID_CMD:
			[[fallthrough]];
		default:
			replyError(fd);
			break;
	}
}

static void __attribute__((constructor)) initUserService() {
	int priority = 256;
	sceUserServiceInitialize(&priority);
}

bool launchApp(const char *titleId, bool block) {
	puts("launching app");
	uint32_t id = -1;
	uint32_t res = sceUserServiceGetForegroundUser(&id);
	if (res != 0) {
		printf("sceUserServiceGetForegroundUser failed: 0x%llx\n", res);
		return false;
	}
	printf("user id %u\n", id);
	Flag flag = block ? Flag_None : SkipLaunchCheck;
	LncAppParam param{sizeof(LncAppParam), id, 0, 0, flag};
	int err = sceLncUtilLaunchApp(titleId, nullptr, &param);
	printf("sceLncUtilLaunchApp returned 0x%llx\n", (uint32_t)err);
	if (err >= 0) {
		return true;
	}
	switch((uint32_t) err) {
		case SCE_LNC_UTIL_ERROR_ALREADY_RUNNING:
			printf("app %s is already running\n", titleId);
			break;
		case SCE_LNC_ERROR_APP_NOT_FOUND:
			printf("app %s not found\n", titleId);
			break;
		default:
			printf("unknown error 0x%llx\n", (uint32_t) err);
			break;
	}
	return false;
}

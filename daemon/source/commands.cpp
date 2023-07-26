#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "dbg/dbg.hpp"
#include "fd.hpp"
#include "launcher.hpp"
#include "servers.hpp"

enum Command : int8_t {
	INVALID_CMD = -1,
	ACTIVE_CMD = 0,
	LAUNCH_CMD,
	PROCLIST_CMD,
	KILL_CMD,
	KILL_APP_CMD
};

static bool launchApp(const char *titleId, bool block=false);
static bool killApp(uint32_t appId);

static void replyError(TcpSocket &sock) {
	const Command cmd = INVALID_CMD;
	if (!sock.isClosed()) {
		sock.write(&cmd, sizeof(cmd));
	}
}

static void replyOk(TcpSocket &sock) {
	const Command cmd = ACTIVE_CMD;
	sock.write(&cmd, sizeof(cmd));
}

void CommandServer::run(TcpSocket &sock) {
	Command cmd = INVALID_CMD;
	if (!sock.read(&cmd, sizeof(cmd))) {
		// failure to read is catastrophic
		sock.close();
		return;
	}


	printf("cmd: %u\n", cmd);

	switch (cmd) {
		case ACTIVE_CMD:
			replyOk(sock);
			break;
		case LAUNCH_CMD:
			// NPXS40000
			static constexpr auto APP_ID_LENGTH = 10;
			char appId[APP_ID_LENGTH];
			if (!sock.read(appId, sizeof(appId) - 1)) {
				replyError(sock);
				break;
			}
			appId[sizeof(appId) - 1] = '\0';
			if (launchApp(appId)) {
				replyOk(sock);
			} else {
				replyError(sock);
			}
			break;
		case PROCLIST_CMD:
			for (auto p : dbg::getProcesses()) {
				printf("%s: %d %s\n", p.name().c_str(), p.pid(), p.path().c_str());
			}
			break;
		/*case KILL_CMD:
			replyOk(sock);
			sock.close();
			break;*/
		case KILL_APP_CMD: {
			uint32_t id{};
			if (!sock.read(&id, sizeof(id))) {
				replyError(sock);
				break;
			}
			printf("id: 0x%lx\n", id);
			if (!killApp(id)) {
				replyError(sock);
				break;
			}
			replyOk(sock);
			break;
		}
		case INVALID_CMD:
			[[fallthrough]];
		default:
			replyError(sock);
			break;
	}
}

static void __attribute__((constructor)) initUserService() {
	static constexpr auto DEFAULT_PRIORITY = 256;
	int priority = DEFAULT_PRIORITY;
	sceUserServiceInitialize(&priority);
}

extern "C" uint32_t sceLncUtilKillApp(uint32_t appId);

static bool killApp(uint32_t appId) {
	uint32_t res = sceLncUtilKillApp(appId);
	printf("sceApplicationKill returned 0x%llx\n", res);
	return true;
}

static bool launchApp(const char *titleId, bool block) {
	puts("launching app");
	uint32_t id = -1;
	uint32_t res = sceUserServiceGetForegroundUser(&id);
	if (res != 0) {
		printf("sceUserServiceGetForegroundUser failed: 0x%llx\n", res);
		return false;
	}
	printf("user id %u\n", id);
	Flag flag = SkipLaunchCheck;
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

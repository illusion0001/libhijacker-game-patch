#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "dbg/dbg.hpp"
#include "elf/elf.hpp"
#include "fd.hpp"
#include "hijacker/hijacker.hpp"
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

static bool launchApp(const char *titleId);
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

static void listProcs() {
	for (const auto &p : dbg::getProcesses()) {
		printf("%d %s\n", p.pid(), p.name().c_str());
	}
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
			listProcs();
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

struct LaunchArgs {
	const char *titleId;
	uint32_t id;
	int *appId;
};

static void *doLaunchApp(void *ptr) {
	UniquePtr<LaunchArgs> args = reinterpret_cast<LaunchArgs*>(ptr);
	Flag flag = Flag_None;
	LncAppParam param{sizeof(LncAppParam), args->id, 0, 0, flag};

	int err = sceLncUtilLaunchApp(args->titleId, nullptr, &param);
	printf("sceLncUtilLaunchApp returned 0x%llx\n", (uint32_t)err);
	if (err >= 0) {
		return nullptr;
	}
	switch ((uint32_t) err) {
		case SCE_LNC_UTIL_ERROR_ALREADY_RUNNING:
			printf("app %s is already running\n", args->titleId);
			break;
		case SCE_LNC_ERROR_APP_NOT_FOUND:
			printf("app %s not found\n", args->titleId);
			break;
		default:
			printf("unknown error 0x%llx\n", (uint32_t) err);
			break;
	}
	return nullptr;
}

static pthread_t launchAppThread(const char *titleId, int *appId) {
	puts("launching app");
	uint32_t id = -1;
	uint32_t res = sceUserServiceGetForegroundUser(&id);
	if (res != 0) {
		printf("sceUserServiceGetForegroundUser failed: 0x%llx\n", res);
		return nullptr;
	}
	printf("user id %u\n", id);

	// the thread will clean this up
	LaunchArgs *args = new LaunchArgs{titleId, id, appId}; // NOLINT(*)
	pthread_t td = nullptr;
	pthread_create(&td, nullptr, doLaunchApp, args);
	return td;
}

/*
static constexpr uintptr_t ENTRYPOINT_OFFSET = 0x70;

struct LoopBuilder {
	static constexpr size_t LOOB_BUILDER_SIZE = 39;
	static constexpr size_t LOOP_BUILDER_TARGET_OFFSET = 11;
	static constexpr size_t LOOP_BUILDER_STACK_PTR_OFFSET = 5;
	uint8_t data[LOOB_BUILDER_SIZE];

	void setTarget(uintptr_t addr) {
		*reinterpret_cast<uintptr_t *>(data + LOOP_BUILDER_TARGET_OFFSET) = addr;
	}
	void setStackPointer(uintptr_t addr) {
		*reinterpret_cast<uint32_t *>(data + LOOP_BUILDER_STACK_PTR_OFFSET) = (uint32_t)addr;
	}
};

static inline constexpr LoopBuilder SLEEP_LOOP{
	// 67 48 89 24 25 xx xx xx xx
	// MOV [SAVED_STACK_POINTER], RSP
	0x67, 0x48, 0x89, 0x24, 0x25, 0x00, 0x00, 0x00, 0x00,

	// // 48 b8 xx xx xx xx xx xx xx xx 48 c7 c7 40 42 0f 00 ff d0 eb eb
	//loop:
	//	MOV RAX, _nanosleep
	0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	// MOV RDI, 1000000000 // 1 second
	0x48, 0xc7, 0xc7, 0x00, 0xca, 0x9a, 0x3b,
	// MOV RSI, 0
	0x48, 0xc7, 0xc6, 0x00, 0x00, 0x00, 0x00,
	// PUSH RDI
	0x57,
	// PUSH RSI
	0x56,
	// CALL RAX
	0xff, 0xd0,
	// JMP loop
	0xeb, 0xe2
};


static uintptr_t getNanosleepOffset(const Hijacker &hijacker) {
	uintptr_t addr = hijacker.getLibKernelFunctionAddress(nid::_nanosleep);
	return addr - hijacker.getLibKernelBase();
}
*/

static int getNextPid(const int lastPid) {
	// get the pid of the new process as soon as it is created
	int pid = lastPid;
	while (pid == lastPid) {
		usleep(1000); // NOLINT(*)
		pid = dbg::getAllPids()[0];
	}
	if (dbg::ProcessInfo{pid}.name() == "ScePfs"_sv) {
		puts("Skipping ScePfs");
		return getNextPid(pid);
	}
	return pid;
}

class GameServer : public TcpServer {

	static constexpr uint16_t PORT = 9050;
	int pid;

	void run(TcpSocket &sock) override;

	void start() noexcept override {
		TcpSocket sock = accept();
		if (sock) {
			run(sock);
		}
	}

	public:
		GameServer(int pid) noexcept : TcpServer(PORT), pid(pid) {
			printf("waiting for game elf on port %d\n", PORT);
		}

		GameServer(const GameServer&) = delete;

		GameServer &operator=(const GameServer&) = delete;

		GameServer(GameServer &&rhs) noexcept : TcpServer((TcpServer&&)rhs), pid(rhs.pid) {
			rhs.pid = 0;
		}

		GameServer &operator=(GameServer &&rhs) = delete;

		~GameServer() noexcept override = default;
};

void GameServer::run(TcpSocket &sock) {
	size_t elfSize = 0;
	if (!sock.read(&elfSize, sizeof(elfSize))) {
		return;
	}

	UniquePtr<uint8_t[]> buf = new uint8_t[elfSize];
	if (!sock.read(buf.get(), sizeof(elfSize))) {
		return;
	}

	auto hijacker = Hijacker::getHijacker(pid);
	if (hijacker == nullptr) {
		sock.println("failed to get hijacker for game process");
		return;
	}

	Elf elf{hijacker.get(), buf.get()};
	if (elf.launch()) {
		puts("launch succeeded");
	} else {
		puts("launch failed");
	}
}

static bool launchApp(const char *titleId) {
	//static constexpr size_t NANOSLEEP_OFFSET = 0x28D0;
	//LoopBuilder loop = SLEEP_LOOP;

	const int lastPid = dbg::getAllPids()[0];

	int appId = 0;
	pthread_t td = launchAppThread(titleId, &appId);
	if (td == nullptr) {
		puts("failed to start thread");
		return false;
	}

	puts("waiting for new process to spawn");

	// get the pid of the new process as soon as it is created
	int pid = getNextPid(lastPid);

	printf("found new pid %d\n", pid);

	UniquePtr<Hijacker> spawned = nullptr;
	dbg::AuthidSwapper idSwapper{dbg::PTRACE_ID};
	{
		// attach to the new process
		dbg::Tracer tracer{pid};

		// run until execve finishes and sends the signal
		tracer.run();

		while (spawned == nullptr) {
			// this should grab it first try but I haven't confirmed yet
			spawned = Hijacker::getHijacker(pid);
		}

		printf("libkernel imagebase: 0x%08llx\n", spawned->getLibKernelBase());

		puts("spawned process obtained");

		puts("success");

		uintptr_t base = 0;
		while (base == 0) {
			// this should also work first try but not confirmed
			base = spawned->getLibKernelBase();
		}

		/*const uintptr_t rsp = spawned->getDataAllocator().allocate(8);
		loop.setStackPointer(rsp);
		loop.setTarget(base + NANOSLEEP_OFFSET);
		base = spawned->imagebase();
		spawned->pSavedRsp = rsp;

		// insert a software breakpoint at the entry point
		// sadly this didn't work :(
		// it won't work because we need to detatch which will cause it to exit

		// force the entrypoint to an infinite loop so that it doesn't start until we're ready
		dbg::write(pid, base + ENTRYPOINT_OFFSET, loop.data, sizeof(loop.data));*/

		puts("joining");
		pthread_join(td, nullptr);

		puts("finished");
		printf("spawned imagebase 0x%08llx\n", base);

		tracer.run();
	}

	GameServer gs{pid};
	gs.TcpServer::run();
	return true;
	//sceLncUtilKillApp
}

#include <errno.h>
#include <ps5/payload_main.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stddef.h>
#include <sys/_stdint.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "dbg/dbg.hpp"
#include "elf/elf.hpp"
#include "fd.hpp"
#include "hijacker/hijacker.hpp"
#include "hijacker/spawner.hpp"
#include "launcher.hpp"
#include "servers.hpp"
#include "util.hpp"

static constexpr auto json = R"({
  "applicationCategoryType": 33554432,
  "localizedParameters": {
    "defaultLanguage": "en-US",
    "en-US": {
      "titleName": "HomebrewDaemon"
    }
  },
)"_sv;

//static constexpr int STUPID_C_ERROR_VALUE = -1;
static constexpr uint32_t ELF_MAGIC = 0x464C457F;

//static const char *EBOOT_PATH = "/app0/eboot.bin";
//static const char *SPAWN_ARGS[] = {NULL};

struct AppStatus {
	static constexpr auto PAD_SIZE = 28;
	unsigned int id;
	unsigned char pad[PAD_SIZE];
};

struct LocalProcessArgs {
	int fds[2];
	int a;
	int b;
	unsigned long long pad[2];
};

extern "C" int sceSystemServiceGetAppStatus(AppStatus *status);
extern "C" uint64_t sceSystemServiceAddLocalProcess(unsigned int id, const char *path, const char **argv, LocalProcessArgs *args);
extern "C" int32_t sceSystemServiceGetAppId(const char *titleId);

enum ProcessType : uint8_t {
	INVALID,
	DAEMON,
	GAME
};

enum class ResponseType : int8_t {
	OK,
	ERROR = -1
};

static bool exists(const char *path) noexcept {
	struct stat st{};
	if (stat(path, &st) == -1) {
		return false;
	}
	return S_ISDIR(st.st_mode);
}

struct TitleId {
	static constexpr size_t TITLEID_SIZE = 10;
	char id[TITLEID_SIZE]{};
	operator StringView() const noexcept {
		return {id, TITLEID_SIZE-1};
	}
	explicit operator bool() const noexcept {
		return id[0] != '\0';
	}
	TitleId &operator++() noexcept {
		++id[TitleId::TITLEID_SIZE-2];
		return *this;
	}
	bool exists() const noexcept {
		String path = "/system_ex/app/"_sv;
		path += id;
		return ::exists(path.c_str());
	}
};

static constexpr TitleId DEFAULT_HOMEBREW_ID{"BREW00000"};
static constexpr TitleId ASTROS_PLAYGROUND_ID{"PPSA01325"};

static pid_t launchApp(const char *titleId, int *appId, ProcessType type);

static TitleId getNextAvailableTitleId() noexcept {
	static constexpr int MAX_HOMEBREW_APPS = 10;
	TitleId res = DEFAULT_HOMEBREW_ID;
	for (uint8_t i = 1; i < MAX_HOMEBREW_APPS; i++) {
		auto appId = sceSystemServiceGetAppId((++res).id);
		if (appId == -1) {
			printf("Next id: %s\n", res.id);
			return res;
		}
	}
	return TitleId{};
}

static bool killApp(uint32_t appId) {
	uint32_t res = sceLncUtilKillApp(appId);
	printf("sceApplicationKill returned 0x%llx\n", res);
	return true;
}

static constexpr int STUPID_C_ERROR = -1;
static constexpr int MKDIR_FLAGS = 0666;

// NOLINTBEGIN(cppcoreguidelines-owning-memory)

static bool copyfile(const char *from, const char *to) {
	struct stat st{};
	if (stat(from, &st) == STUPID_C_ERROR) {
		puts(strerror(errno));
		return false;
	}
	UniquePtr<uint8_t[]> buf = new uint8_t[st.st_size];
	FILE *fp = fopen(from, "rb");
	if (fp == nullptr) {
		puts("open failed");
		puts(strerror(errno));
		return false;
	}
	fread(buf.get(), 1, st.st_size, fp);
	fclose(fp);
	fp = fopen(to, "wb+");
	if (fp == nullptr) {
		puts("open failed");
		puts(strerror(errno));
		return false;
	}
	fwrite(buf.get(), 1, st.st_size, fp);
	fclose(fp);
	return true;
}

static bool mkdir(const char *path) {
	if (::mkdir(path, MKDIR_FLAGS) == STUPID_C_ERROR) {
		const int err = errno;
		if (err != EEXIST) {
			puts(strerror(errno));
			return false;
		}
	}
	return true;
}

static bool makeHomebrewApp(const StringView &titleId) {
	String path = "/system_ex/app/"_sv;
	path += titleId;
	if (mkdir(path.c_str(), MKDIR_FLAGS) == STUPID_C_ERROR) {
		const int err = errno;
		if (err != EEXIST) {
			perror("makenewapp mkdir");
			return false;
		}
		return true;
	}
	String ebootPath = path + "/eboot.bin";
	if (!copyfile("/system_ex/app/NPXS40028/eboot.bin", ebootPath.c_str())) {
		puts("failed to copy redis eboot.bin");
		return false;
	}

	String sysPath = path + "/sce_sys"_sv;
	if (!mkdir(sysPath.c_str())) {
		return false;
	}

	String newJsonPath = sysPath + "/param.json"_sv;
	FILE *fp = fopen(newJsonPath.c_str(), "w+");
	if (fp == nullptr) {
		perror("fopen failed");
		return false;
	}
	if (fwrite(json.c_str(), 1, json.length(), fp) != json.length()) {
		puts("failed to write param.json");
		fclose(fp);
		return false;
	}

	static constexpr auto JSON_TITLE_ID = "  \"titleId\": \""_sv;
	if (fwrite(JSON_TITLE_ID, 1, JSON_TITLE_ID.length(), fp) != JSON_TITLE_ID.length()) {
		puts("failed to write param.json");
		fclose(fp);
		return false;
	}

	if (fwrite(titleId, 1, titleId.length(), fp) != titleId.length()) {
		puts("failed to write param.json");
		fclose(fp);
		return false;
	}

	static constexpr auto JSON_END = "\"\n}\n"_sv;
	if (fwrite(JSON_END, 1, JSON_END.length(), fp) != JSON_END.length()) {
		puts("failed to write param.json");
		fclose(fp);
		return false;
	}

	fclose(fp);
	return true;
}


// NOLINTEND(cppcoreguidelines-owning-memory)

void ElfServer::run(TcpSocket &sock) {
	puts("ElfServer::run");
	constexpr auto MAX_NAME_SIZE = 32;
	ResponseType response = ResponseType::ERROR;
	ProcessType type = INVALID;
	if (!sock.read(&type, sizeof(type))) {
		puts("sock read failed");
		return;
	}

	if (type != DAEMON && type != GAME) {
		sock.write(&response, sizeof(response));
		puts("Unexpected process type");
		return;
	}

	printf("elf type %d\n", type);

	char name[MAX_NAME_SIZE]{};
	if (!sock.read(name, sizeof(name))) {
		return;
	}

	printf("name: %s\n", name);

	size_t elfSize = 0;
	if (!sock.read(&elfSize, sizeof(elfSize))) {
		return;
	}

	printf("elf size %llu\n", elfSize);

	UniquePtr<uint8_t[]> buf = new uint8_t[elfSize];
	if (!sock.read(buf.get(), sizeof(elfSize))) {
		return;
	}
	if (*reinterpret_cast<uint32_t *>(buf.get()) != ELF_MAGIC) {
		sock.write(&response, sizeof(response));
		puts("invalid elf");
		return;
	}

	TitleId id = type == GAME ? ASTROS_PLAYGROUND_ID : getNextAvailableTitleId();

	if (type == DAEMON) {
		// check if titleid exists and if not create it
		if (!id) {
			sock.write(&response, sizeof(response));
			puts("too many homebrew apps are already running");
			return;
		}

		if (!id.exists()) {

			if (!makeHomebrewApp(id)) {
				sock.write(&response, sizeof(response));
				puts("failed to create new homebrew app in /system_ex/app");
				return;
			}
		}
	}

	int appId = 0;
	pid_t pid = launchApp(id.id, &appId, type);
	if (pid == 0) {
		puts("spawn failed");
		sock.write(&response, sizeof(response));
	}

	auto hijacker = Hijacker::getHijacker(pid);
	if (hijacker == nullptr) {
		sock.write(&response, sizeof(response));
		puts("spawn failed");
		return;
	}

	puts("getting saved stack pointer");
	while (hijacker->getSavedRsp() == 0) {
		usleep(1);
	}

	puts("setting process name");
	hijacker->getProc()->setName(name);
	__builtin_printf("new process %s pid %d\n", hijacker->getProc()->getSelfInfo()->name, hijacker->getPid());
	puts("jailbreaking new process");
	hijacker->jailbreak();

	Elf elf(hijacker.get(), buf.get());

	if (!elf.launch()) {
		sock.write(&response, sizeof(response));
		puts("failed to load elf");
		killApp(appId);
		return;
	}

	printf("successfully launched %s\n", name);

	response = ResponseType::OK;

	sock.write(&response, sizeof(response));
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

static int getNextPid(const int lastPid, ProcessType type) {
	// get the pid of the new process as soon as it is created
	int pid = lastPid;
	while (pid == lastPid) {
		usleep(1000); // NOLINT(*)
		pid = dbg::getAllPids()[0];
	}
	if (type == DAEMON) {
		return pid;
	}
	if (dbg::ProcessInfo{pid}.name() == "ScePfs"_sv) {
		puts("Skipping ScePfs");
		return getNextPid(pid, type);
	}
	return pid;
}

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


static uintptr_t getNanosleepOffset(int pid) {
	auto hijacker = Hijacker::getHijacker(pid);
	uintptr_t addr = hijacker->getLibKernelFunctionAddress(nid::_nanosleep);
	return addr - hijacker->getLibKernelBase();
}

extern "C" int cpuset_setaffinity(cpulevel_t level, cpuwhich_t which, id_t id, size_t setsize, const void *mask);
extern "C" int rtprio_thread(int function, lwpid_t lwpid, void *rtp);

static void setaffinity(int value) noexcept {
	uint8_t mask[16]{};
	mask[1] = value;
	uint16_t prio[]{2, 0x100};
	cpuset_setaffinity(3, 1, -1, 16, mask);
	rtprio_thread(1, 0, prio);
}

static pid_t launchApp(const char *titleId, int *appId, ProcessType type) {
	//static constexpr size_t NANOSLEEP_OFFSET = 0x28D0;
	setaffinity(1);
	LoopBuilder loop = SLEEP_LOOP;
	const uintptr_t nanosleepOffset = type == DAEMON ? getNanosleepOffset(getpid()) : 0;

	const int lastPid = dbg::getAllPids()[0];

	pthread_t td = launchAppThread(titleId, appId);
	if (td == nullptr) {
		puts("failed to start thread");
		return false;
	}

	puts("waiting for new process to spawn");

	// get the pid of the new process as soon as it is created
	int pid = getNextPid(lastPid, type);

	printf("found new pid %d\n", pid);

	UniquePtr<Hijacker> spawned = nullptr;
	{
		// attach to the new process
		dbg::Tracer tracer{pid};

		// run until execve finishes and sends the signal
		tracer.run();

		while (spawned == nullptr) {
			// this should grab it first try but I haven't confirmed yet
			spawned = Hijacker::getHijacker(pid);
		}

		uintptr_t base = 0;
		while (base == 0) {
			// this should also work first try but not confirmed
			base = spawned->getLibKernelBase();
		}

		if (type == DAEMON) {
			const uintptr_t rsp = spawned->getDataAllocator().allocate(8);
			loop.setStackPointer(rsp);
			loop.setTarget(base + nanosleepOffset);
			base = spawned->imagebase();
			spawned->pSavedRsp = rsp;

			// force the entrypoint to an infinite loop so that it doesn't start until we're ready
			dbg::write(pid, base + ENTRYPOINT_OFFSET, loop.data, sizeof(loop.data));
		}

		puts("joining");
		pthread_join(td, nullptr);

		puts("finished");
		printf("spawned imagebase 0x%08llx\n", base);
	}

	//GameServer gs{pid};
	//gs.TcpServer::run();
	return true;
	//sceLncUtilKillApp
}


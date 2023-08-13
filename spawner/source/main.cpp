#include "backtrace.hpp"
#include "dbg.hpp"
#include "dbg/dbg.hpp"
#include "elf/elf.hpp"
#include "hijacker/hijacker.hpp"
#include "kernel.hpp"
#include "hijacker.hpp"
#include "kernel/kernel.hpp"
#include "kernel/proc.hpp"
#include "offsets.hpp"
#include "util.hpp"
#include <elf.h>
#include <sys/_pthreadtypes.h>
#include <sys/_stdint.h>
#include <unistd.h>
#include <pthread.h>


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
	#include <sys/ptrace.h>
	#include <sys/wait.h>
	#include <machine/reg.h>
	ssize_t _read(int, void *, size_t);
}


static constexpr int LOGGER_PORT = 9021;

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
					return false;
				}
				size -= read;
				buf += read;
			}
			return true;
		}

		void release() { fd = -1; }
};

extern "C" const unsigned int daemon_size;
extern "C" uint8_t daemon_start[]; // NOLINT(*)

static bool runElf(Hijacker *hijacker) {

	Elf elf{hijacker, daemon_start};

	if (elf.launch()) {
		puts("launch succeeded");
		return true;
	}
	puts("launch failed");
	return false;
}

static bool load(UniquePtr<Hijacker> &redis) {
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
		return true;
	}
	return false;
}

static bool spawn(Hijacker *hijacker) {
	Spawner spawner{*hijacker};

	puts("spawning new SceRedisServer process");
	auto redis = spawner.bootstrap(*hijacker);

	if (redis == nullptr) {
		puts("failed to spawn new redis server process");
		return false;
	}
	return load(redis);
}

[[maybe_unused]] static void __attribute__((naked, noinline)) clearFramePointer() {
	// this clears the frame pointer so we stop the backtrace at the start of our code
	__asm__ volatile(
		"mov $0, %rbp\n"
		"ret\n"
	);
}

static int initStdout() {
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

		if (bind(sock, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) != 0) {
			return -1;
		}

		if (listen(sock, 1) != 0) {
			return -1;
		}

		struct sockaddr client_addr{};
		socklen_t addr_len = sizeof(client_addr);
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
		Stdout(const Stdout&) = delete;
		Stdout &operator=(const Stdout&) = delete;
		Stdout(Stdout&&) = delete;
		Stdout &operator=(Stdout&&) = delete;
		~Stdout() {
			if (fd != -1) {
				close(STDOUT);
				close(STDERR);
			}
		}
};

extern "C" const uint8_t __text_start __attribute__((weak));
extern "C" const Elf64_Rela __rela_start[] __attribute__((weak));
extern "C" const Elf64_Rela __rela_stop[] __attribute__((weak));

static bool hasUnprocessedRelocations() {
	if (&__rela_start[0] != &__rela_stop[0]) {
		const Elf64_Rela __restrict *it = __rela_start;
		auto ptr = reinterpret_cast<const char *const *>(&__text_start + it->r_offset);
		return *ptr == 0;
	}
	return false;
}

static bool processRelocations() {
	const uintptr_t imagebase = reinterpret_cast<uintptr_t>(&__text_start);
	for (const Elf64_Rela __restrict *it = __rela_start; it != __rela_stop; it++) {
		if (ELF64_R_TYPE(it->r_info) != R_X86_64_RELATIVE) [[unlikely]] {
			printf("unexpected relocation type %d\n", ELF64_R_TYPE(it->r_info));
			return false;
		}
		*reinterpret_cast<uint8_t**>(imagebase + it->r_offset) = reinterpret_cast<uint8_t*>(imagebase + it->r_addend); // NOLINT(*)
	}
	return true;
}

extern "C" const uintptr_t kernel_base;
static constexpr size_t QAFLAGS_SIZE = 16;

struct HookBuilder {
	static constexpr size_t CODE_SIZE = 92;
	static constexpr size_t EXECVE_OFFSET = 0x4D;
	uint8_t data[CODE_SIZE];

	void setExecve(uintptr_t addr) {
		*reinterpret_cast<uintptr_t *>(data + EXECVE_OFFSET) = addr;
	}
};

// NOLINTBEGIN(*)

struct Args {
	int pid;
	uintptr_t execve;
	uintptr_t sceKernelDlsym;

	Args(const UniquePtr<Hijacker> &hijacker) :
			pid(0), execve(hijacker->getLibKernelFunctionAddress(nid::execve)),
			sceKernelDlsym(hijacker->getLibKernelFunctionAddress(nid::sceKernelDlsym)) {
	}
};

struct ShellcodeBuilder {
	static constexpr size_t SHELLCODE_SIZE = 73;
	static constexpr size_t EXECVE_ADDR_OFFSET = 2;
	static constexpr size_t NANOSLEEP_ADDR_OFFSET = 12;

	uint8_t shellcode[SHELLCODE_SIZE];

	void setExecveAddr(uintptr_t addr) noexcept {
		*reinterpret_cast<uintptr_t*>(shellcode + EXECVE_ADDR_OFFSET) = addr;
	}

	void setNanosleepAddr(uintptr_t addr) noexcept {
		*reinterpret_cast<uintptr_t*>(shellcode + NANOSLEEP_ADDR_OFFSET) = addr;
	}
};

// insert 0x48, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

static constexpr ShellcodeBuilder BUILDER_TEMPLATE{{
	0x48, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV RCX, execve
	0x49, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV R8, _nanosleep
	0x41, 0x57, 0x41, 0x56, 0x41, 0x54, 0x53, 0x50, 0x49, 0x89, 0xff, 0xbf, 0x00, 0x94, 0x35, 0x77,
    0x49, 0x89, 0xcc, 0x48, 0x89, 0xd3, 0x49, 0x89, 0xf6, 0x41, 0xff, 0xd0, 0x4c, 0x89, 0xff, 0x4c,
    0x89, 0xf6, 0x48, 0x89, 0xda, 0x4c, 0x89, 0xe0, 0x48, 0x83, 0xc4, 0x08, 0x5b, 0x41, 0x5c, 0x41,
    0x5e, 0x41, 0x5f, 0xff, 0xe0
}};

// NOLINTEND(*)

struct Helper {
	UniquePtr<Hijacker> hijacker;
	uintptr_t pidAddr;
};

Helper patchSyscore() {
	puts("patching syscore execve");
	auto hijacker = Hijacker::getHijacker("SceSysCore.elf"_sv);
	if (hijacker == nullptr) {
		puts("failed to get SceSysCore.elf");
		return {nullptr, 0};
	}
	uintptr_t code = hijacker->getTextAllocator().allocate(ShellcodeBuilder::SHELLCODE_SIZE);
	//static constexpr Nid printfNid{"hcuQgD53UxM"};
	//static constexpr Nid amd64_set_fsbaseNid{"3SVaehJvYFk"};
	auto execve = hijacker->getLibKernelFunctionAddress(nid::execve);
	if (execve == 0) {
		puts("failed to locate execve");
		return {nullptr, 0};
	}
	auto _nanosleep = hijacker->getLibKernelFunctionAddress(nid::_nanosleep);
	if (_nanosleep == 0) {
		puts("failed to locate execve");
		return {nullptr, 0};
	}
	auto meta = hijacker->getEboot()->getMetaData();
	const auto &plttab = meta->getPltTable();
	auto index = meta->getSymbolTable().getSymbolIndex(nid::execve);
	if (index == -1) {
		puts("execve import not found");
		return {nullptr, 0};
	}
	for (const auto &plt : plttab) {
		if (ELF64_R_SYM(plt.r_info) == index) {
			ShellcodeBuilder builder = BUILDER_TEMPLATE;
			builder.setExecveAddr(execve);
			builder.setNanosleepAddr(_nanosleep);
			hijacker->write(code, builder.shellcode);
			uintptr_t addr = hijacker->getEboot()->imagebase() + plt.r_offset;
			hijacker->write<uintptr_t>(addr, code);
			return {(UniquePtr<Hijacker>&&)hijacker, 0};
		}
	}
	return {nullptr, 0};
}

extern "C" int sceUserServiceGetForegroundUser(uint32_t *userId);

enum Flag : uint64_t {
    Flag_None = 0,
    SkipLaunchCheck = 1,
    SkipResumeCheck = 1,
    SkipSystemUpdateCheck = 2,
    RebootPatchInstall = 4,
    VRMode = 8,
    NonVRMode = 16,
	Pft = 32UL,
	RaIsConfirmed = 64UL,
	ShellUICheck = 128UL
};


struct LncAppParam {
    uint32_t sz;
    uint32_t user_id;
    uint32_t app_opt;
    uint64_t crash_report;
    Flag check_flag;
};

extern "C" int sceKernelLoadStartModule(const char *name, size_t argc, const void *argv, uint32_t flags, void *unknown, int *result);
extern "C" int sceKernelDlsym(uint32_t lib, const char *name, void **fun);

static constexpr uint32_t SCE_LNC_UTIL_ERROR_ALREADY_RUNNING = 0x8094000c;
static constexpr uint32_t SCE_LNC_ERROR_APP_NOT_FOUND = 0x80940031;
extern "C" int sceSysmoduleLoadModuleInternal(uint32_t);
extern "C" int sceSysmoduleLoadModuleByNameInternal(const char *fname, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

static uint32_t getLibUserService() {
	static constexpr uint32_t USER_SERVICE_ID = 0x80000011;
	uint32_t handle =  sceSysmoduleLoadModuleInternal(USER_SERVICE_ID);
	if (handle != 0) {
		return handle;
	}
	auto hijacker = Hijacker::getHijacker(getpid());
	if (hijacker == nullptr) {
		return 0;
	}
	for (const auto &lib : hijacker->getLibs()) {
		if (lib->getPath().endswith("libSceUserService.sprx"_sv)) {
			return lib->handle();
		}
	}
	return 0;
}

static uint32_t getLibSystemService() {
	static constexpr uint32_t SYSTEM_SERVICE_ID = 0x80000010;
	uint32_t handle =  sceSysmoduleLoadModuleInternal(SYSTEM_SERVICE_ID);
	if (handle != 0) {
		return handle;
	}
	auto hijacker = Hijacker::getHijacker(getpid());
	if (hijacker == nullptr) {
		return 0;
	}
	for (const auto &lib : hijacker->getLibs()) {
		if (lib->getPath().endswith("libSceSystemService.sprx"_sv)) {
			return lib->handle();
		}
	}
	return 0;
}

static void *getSceLncUtilLaunchApp() {
	static constexpr Nid nid{"+nRJUD-7qCk"};
	auto hijacker = Hijacker::getHijacker(getpid());
	if (hijacker == nullptr) {
		return nullptr;
	}
	for (const auto &lib : hijacker->getLibs()) {
		if (lib->getPath().endswith("libSceSystemService.sprx"_sv)) {
			return (void*)hijacker->getFunctionAddress(lib.get(), nid);
		}
	}
	return nullptr;
}

static void *getSceLncUtilKillApp() {
	static constexpr Nid nid{"SZ2uH5Abws8"};
	auto hijacker = Hijacker::getHijacker(getpid());
	if (hijacker == nullptr) {
		return nullptr;
	}
	for (const auto &lib : hijacker->getLibs()) {
		if (lib->getPath().endswith("libSceSystemService.sprx"_sv)) {
			return (void*)hijacker->getFunctionAddress(lib.get(), nid);
		}
	}
	return nullptr;
}

struct LaunchArgs {
	const char *titleId;
	uint32_t id;
	int *appId;
};

static void *doLaunchApp(void *ptr) {
	UniquePtr<LaunchArgs> args = (LaunchArgs *)ptr;
	Flag flag = Flag_None;
	LncAppParam param{sizeof(LncAppParam), args->id, 0, 0, flag};
	using ftype = int (*)(const char* tid, const char* argv[], LncAppParam* param);
	int (*sceLncUtilLaunchApp)(const char* tid, const char* argv[], LncAppParam* param) = (ftype) getSceLncUtilLaunchApp();
	if (sceLncUtilLaunchApp == nullptr) {
		puts("failed to get address of sceLncUtilLaunchApp");
		return nullptr;
	}
	puts("calling sceLncUtilLaunchApp");
	int err = sceLncUtilLaunchApp(args->titleId, nullptr, &param);
	*args->appId = err;
	printf("sceLncUtilLaunchApp returned 0x%llx\n", (uint32_t)err);
	if (err >= 0) {
		return nullptr;
	}
	switch((uint32_t) err) {
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

static pthread_t launchApp(const char *titleId, int *appId) {
	puts("launching app");
	uint32_t id = -1;
	uint32_t libUserService = getLibUserService();
	printf("libUserService 0x%08lx\n", libUserService);
	if (libUserService == 0) {
		return nullptr;
	}
	static constexpr auto DEFAULT_PRIORITY = 256;
	int priority = DEFAULT_PRIORITY;
	uint32_t (*sceUserServiceInitialize)(int*) = nullptr;
	sceKernelDlsym(libUserService, "sceUserServiceInitialize", (void**)&sceUserServiceInitialize);
	if (sceUserServiceInitialize == nullptr) {
		puts("failed to resolve sceUserServiceInitialize");
		return nullptr;
	}
	sceUserServiceInitialize(&priority);
	uint32_t (*sceUserServiceGetForegroundUser)(uint32_t *) = nullptr;
	sceKernelDlsym(libUserService, "sceUserServiceGetForegroundUser", (void**)&sceUserServiceGetForegroundUser);
	if (sceUserServiceGetForegroundUser == nullptr) {
		puts("failed to resolve sceUserServiceGetForegroundUser");
		return nullptr;
	}
	uint32_t res = sceUserServiceGetForegroundUser(&id);
	if (res != 0) {
		printf("sceUserServiceGetForegroundUser failed: 0x%llx\n", res);
		return nullptr;
	}
	printf("user id %u\n", id);

	uint32_t libSystemService = getLibSystemService();
	printf("libSystemService 0x%08lx\n", libSystemService);
	LaunchArgs *args = new LaunchArgs{titleId, id, appId}; // NOLINT(*)
	pthread_t td = nullptr;
	pthread_create(&td, nullptr, doLaunchApp, args);
	return td;
}

static void dumpPids() {
	for (const auto &p : dbg::getProcesses()) {
		printf("%d %s\n", p.pid(), p.name().c_str());
	}
}

static constexpr uintptr_t ENTRYPOINT_OFFSET = 0x70;
static constexpr size_t LOOB_BUILDER_SIZE = 30;
static constexpr size_t LOOP_BUILDER_TARGET_OFFSET = 11;
static constexpr size_t LOOP_BUILDER_STACK_PTR_OFFSET = 5;

struct LoopBuilder {
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
	// MOV EDI, 1000000000 // 1 second
	0x48, 0xc7, 0xc7, 0x00, 0xca, 0x9a, 0x3b,
	// CALL RAX
	0xff, 0xd0,
	// JMP loop
	0xeb, 0xeb
};

static uintptr_t getNanosleepOffset(const Hijacker &hijacker) {
	uintptr_t addr = hijacker.getLibKernelFunctionAddress(nid::_nanosleep);
	return addr - hijacker.getLibKernelBase();
}

static reg getRegs(int pid) {
	reg result{};
	if (ptrace(PT_GETREGS, pid, (caddr_t)&result, 0) < 0) {
		perror("ptrace getRegs");
	}
	return result;
}

namespace dbg {

extern void _dbg_init();

}

extern "C" int main() {
	Stdout dummy{};
	//ptrace(PT_ATTACH, pid, 0, 0);
	///clearFramePointer();
	puts("main entered");
	if (hasUnprocessedRelocations()) {
		puts("fixing unprocessed relocations for spawner.elf");
		processRelocations();
	}
	dbg::_dbg_init();

	LoopBuilder loop = SLEEP_LOOP;

	uint8_t qaflags[QAFLAGS_SIZE];
	kread<QAFLAGS_SIZE>(kernel_base + offsets::qa_flags(), qaflags);
	qaflags[1] |= 1 | 2;
	kwrite<QAFLAGS_SIZE>(kernel_base + offsets::qa_flags(), qaflags);

	auto syscore = patchSyscore();
	const uintptr_t nanosleepOffset = getNanosleepOffset(*syscore.hijacker);
	//puts("spawning daemon");
	puts("waiting for new process to spawn");
	int appId = 0;
	pthread_t td = launchApp("BREW00000", &appId);
	if (td == nullptr) {
		puts("failed to start thread");
		return 0;
	}

	const int lastPid = dbg::getAllPids()[0];

	int pid = lastPid;
	while (pid == lastPid) {
		usleep(1000); // NOLINT(*)
		pid = dbg::getAllPids()[0];
	}

	//syscore.hijacker = Hijacker::getHijacker(pid);

	uint64_t id = dbg::setAuthId(dbg::PTRACE_ID);

	int err = ptrace(PT_ATTACH, pid, 0, 0);
	if (err < 0) {
		perror("ptrace");
		dbg::setAuthId(id);
		return 0;
	}

	int status = 0;
	if (waitpid(pid, &status, 0) < 0) {
		perror("waitpid");
		dbg::setAuthId(id);
		return -1;
	}

	printf("status %d\n", status);
	puts("attatched");
	dumpPids();

	err = ptrace(PT_CONTINUE, pid, (caddr_t)1, 0);
	if (err < 0) {
		perror("ptrace");
		dbg::setAuthId(id);
		return 0;
	}

	if (waitpid(pid, &status, 0) < 0) {
		perror("waitpid");
		dbg::setAuthId(id);
		return -1;
	}

	UniquePtr<Hijacker> spawned = nullptr;
	puts("waiting for process to spawn");
	while (spawned == nullptr) {
		spawned = Hijacker::getHijacker(pid);
		//dumpPids();
	}

	printf("status %d\n", status);
	puts("continued");
	reg r = getRegs(pid);
	printf("libkernel imagebase: 0x%08llx\n", spawned->getLibKernelBase());
	printf("rip: 0x%08llx\n", r.r_rip);
	dumpPids();

	puts("spawned process obtained");

	puts("success");
	//dbg::setAuthId(id);

	uintptr_t base = 0;
	while (base == 0) {
		base = spawned->getLibKernelBase();
	}

	const uintptr_t rsp = spawned->getDataAllocator().allocate(8);
	loop.setStackPointer(rsp);
	loop.setTarget(base + nanosleepOffset);
	base = spawned->imagebase();
	spawned->pSavedRsp = rsp;

	// insert a software breakpoint at the entry point
	// sadly this didn't work :(
	/*
	static constexpr uint8_t INT3 = 0xcc;
	uint8_t breakpoint[]{INT3};
	const uintptr_t entry = base + ENTRYPOINT_OFFSET;
	dbg::write(pid, entry, breakpoint, sizeof(breakpoint));

	while (true) {
		err = ptrace(PT_CONTINUE, pid, (caddr_t)1, 0);
		if (err < 0) {
			perror("ptrace");
			dbg::setAuthId(id);
			return 0;
		}

		if (waitpid(pid, &status, 0) < 0) {
			perror("waitpid");
			dbg::setAuthId(id);
			return -1;
		}
		reg r = getRegs(pid);
		uint8_t inst = 0;
		dbg::read(pid, r.r_rip-1, sizeof(inst));
		if (inst == INT3) {
			puts("stopped at software breakpoint");
			break;
		}
		printf("rip: 0x%08llx\n", r.r_rip);
	}
	*/

	// force the entrypoint to an infinite loop so that it doesn't start until we're ready
	dbg::write(pid, base + ENTRYPOINT_OFFSET, loop.data, sizeof(loop.data));

	err = ptrace(PT_DETACH, pid, 0, 0);
	if (err < 0) {
		perror("ptrace");
		dbg::setAuthId(id);
		return 0;
	}

	dbg::setAuthId(id);

	pthread_join(td, nullptr);

	puts("finished");

	if (!load(spawned)) {
		puts("failed to load elf into new process");
		using ftype = uint32_t (*)(uint32_t);
		auto sceLncUtilKillApp = (ftype) getSceLncUtilKillApp(); // NOLINT(*)
		sceLncUtilKillApp(appId);
	}

	dumpPids();

	return 0;

	/*
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

	// forcefully trying like this can cause a panic on shutdown after x amount of failed attempts
	while (!spawn(hijacker.get())) {
		if (Hijacker::getHijacker("SceRedisServer"_sv) == nullptr) {
			puts("SceRedisServer died, restart required");
			break;
		}
		puts("spawn failed retrying...");
	}

	puts("spawner.elf finished");

	return 0;
	*/
}

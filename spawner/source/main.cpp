#include "backtrace.hpp"
#include "dbg.hpp"
#include "dbg/dbg.hpp"
#include "elf/elf.hpp"
#include "hijacker/hijacker.hpp"
#include "kernel.hpp"
#include "hijacker.hpp"
#include "kernel/kernel.hpp"
#include "kernel/proc.hpp"
#include "nid.hpp"
#include "offsets.hpp"
#include "util.hpp"
#include <elf.h>
#include <sys/_pthreadtypes.h>
#include <sys/_stdint.h>
#include <sys/un.h>
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
		void close() {
			if (fd != -1) {
				::close(fd);
				fd = -1;
			}
		}
};

extern "C" const unsigned int daemon_size;
extern "C" uint8_t daemon_start[]; // NOLINT(*)

static bool runElf(Hijacker *hijacker) {

	Elf elf{hijacker, daemon_start};

	if (!elf) {
		return false;
	}

	if (elf.launch()) {
		puts("launch succeeded");
		return true;
	}
	puts("launch failed");
	return false;
}

static bool load(UniquePtr<Hijacker> &redis) {
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

struct ShellcodeBuilder {
	static constexpr size_t SHELLCODE_SIZE = 538;
	static constexpr size_t RFORK_THREAD_ADDR_OFFSET = 2;
	static constexpr size_t EXTRA_STUFF_ADDR_OFFSET = 12;

	uint8_t shellcode[SHELLCODE_SIZE];

	void setRforkThreadAddr(uintptr_t addr) noexcept {
		*reinterpret_cast<uintptr_t*>(shellcode + RFORK_THREAD_ADDR_OFFSET) = addr;
	}

	void setExtraStuffAddr(uintptr_t addr) noexcept {
		*reinterpret_cast<uintptr_t*>(shellcode + EXTRA_STUFF_ADDR_OFFSET) = addr;
	}
};

// insert 0x48, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

static constexpr ShellcodeBuilder BUILDER_TEMPLATE{{
	0x49, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV rfork_thread, R8
	0x49, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MOV &stuff, R9
    0x55, 0x41, 0x57, 0x41, 0x56, 0x41, 0x55, 0x41, 0x54, 0x53, 0x48, 0x81, 0xec, 0x98, 0x00, 0x00,
    0x00, 0x48, 0x8b, 0x41, 0x08, 0x48, 0x89, 0xcb, 0x49, 0x89, 0xd6, 0x49, 0x89, 0xf7, 0x89, 0xfd,
    0x48, 0x85, 0xc0, 0x0f, 0x84, 0xc2, 0x00, 0x00, 0x00, 0x49, 0xbd, 0x2f, 0x73, 0x79, 0x73, 0x74,
    0x65, 0x6d, 0x5f, 0x4c, 0x39, 0x28, 0x0f, 0x85, 0xaf, 0x00, 0x00, 0x00, 0x48, 0xb9, 0x65, 0x78,
    0x2f, 0x61, 0x70, 0x70, 0x2f, 0x42, 0x48, 0x39, 0x48, 0x08, 0x0f, 0x85, 0x9b, 0x00, 0x00, 0x00,
    0xb9, 0xff, 0xff, 0xff, 0x00, 0x23, 0x48, 0x10, 0x81, 0xf9, 0x52, 0x45, 0x57, 0x00, 0x0f, 0x85,
    0x87, 0x00, 0x00, 0x00, 0x41, 0x8b, 0x01, 0x4d, 0x89, 0xcc, 0x4c, 0x89, 0x44, 0x24, 0x10, 0x83,
    0xf8, 0xff, 0x0f, 0x84, 0x92, 0x00, 0x00, 0x00, 0x48, 0x8d, 0x74, 0x24, 0x18, 0x89, 0xc7, 0xba,
    0x10, 0x00, 0x00, 0x00, 0xb9, 0x00, 0x00, 0x02, 0x00, 0xc7, 0x44, 0x24, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x48, 0xc7, 0x44, 0x24, 0x20, 0x00, 0x00, 0x00, 0x00, 0xc7, 0x44, 0x24, 0x1c, 0x00, 0x00,
    0x00, 0x00, 0x89, 0x44, 0x24, 0x0c, 0x41, 0xff, 0x54, 0x24, 0x30, 0x48, 0x83, 0xf8, 0xff, 0x74,
    0x78, 0x8b, 0x7c, 0x24, 0x0c, 0x48, 0x8d, 0x74, 0x24, 0x28, 0xba, 0x04, 0x00, 0x00, 0x00, 0xb9,
    0x00, 0x00, 0x02, 0x00, 0xc7, 0x44, 0x24, 0x28, 0x00, 0x00, 0x00, 0x00, 0x41, 0xff, 0x54, 0x24,
    0x38, 0x48, 0x83, 0xf8, 0xff, 0x0f, 0x84, 0xee, 0x00, 0x00, 0x00, 0x83, 0x7c, 0x24, 0x28, 0x01,
    0x0f, 0x84, 0x9d, 0x00, 0x00, 0x00, 0xe9, 0xde, 0x00, 0x00, 0x00, 0x89, 0xef, 0x4c, 0x89, 0xfe,
    0x4c, 0x89, 0xf2, 0x48, 0x89, 0xd9, 0x48, 0x81, 0xc4, 0x98, 0x00, 0x00, 0x00, 0x5b, 0x41, 0x5c,
    0x41, 0x5d, 0x41, 0x5e, 0x41, 0x5f, 0x5d, 0x41, 0xff, 0xe0, 0xbf, 0x01, 0x00, 0x00, 0x00, 0xbe,
    0x01, 0x00, 0x00, 0x00, 0x31, 0xd2, 0x41, 0xff, 0x54, 0x24, 0x18, 0x41, 0x89, 0x04, 0x24, 0x83,
    0xf8, 0xff, 0x75, 0x23, 0xe9, 0xb6, 0x00, 0x00, 0x00, 0xbf, 0x01, 0x00, 0x00, 0x00, 0xbe, 0x01,
    0x00, 0x00, 0x00, 0x31, 0xd2, 0x41, 0xff, 0x54, 0x24, 0x18, 0x41, 0x89, 0x04, 0x24, 0x83, 0xf8,
    0xff, 0x0f, 0x84, 0x90, 0x00, 0x00, 0x00, 0x48, 0xb9, 0x74, 0x6d, 0x70, 0x2f, 0x49, 0x50, 0x43,
    0x00, 0x48, 0x8d, 0x74, 0x24, 0x28, 0x89, 0xc7, 0xba, 0x11, 0x00, 0x00, 0x00, 0xc6, 0x44, 0x24,
    0x28, 0x00, 0xc6, 0x44, 0x24, 0x29, 0x01, 0x4c, 0x89, 0x6c, 0x24, 0x2a, 0x41, 0x89, 0xc5, 0x48,
    0x89, 0x4c, 0x24, 0x32, 0x41, 0xff, 0x54, 0x24, 0x28, 0x83, 0xf8, 0xff, 0x74, 0x51, 0x44, 0x89,
    0x6c, 0x24, 0x0c, 0x48, 0x8b, 0x44, 0x24, 0x10, 0x49, 0x8b, 0x54, 0x24, 0x08, 0x89, 0xef, 0x4c,
    0x89, 0xfe, 0x48, 0x89, 0xd9, 0xff, 0xd0, 0x8b, 0x7c, 0x24, 0x0c, 0x48, 0x8d, 0x74, 0x24, 0x28,
    0xba, 0x10, 0x00, 0x00, 0x00, 0xb9, 0x00, 0x00, 0x02, 0x00, 0xc7, 0x44, 0x24, 0x28, 0x01, 0x00,
    0x00, 0x00, 0x89, 0x44, 0x24, 0x2c, 0x41, 0x89, 0xc5, 0x4c, 0x89, 0x74, 0x24, 0x30, 0x41, 0xff,
    0x54, 0x24, 0x30, 0x41, 0x83, 0xfd, 0xff, 0x75, 0x28, 0x8b, 0x7c, 0x24, 0x0c, 0xeb, 0x03, 0x44,
    0x89, 0xef, 0x41, 0xff, 0x54, 0x24, 0x20, 0x41, 0xc7, 0x04, 0x24, 0xff, 0xff, 0xff, 0xff, 0x89,
    0xef, 0x4c, 0x89, 0xfe, 0x4c, 0x89, 0xf2, 0x48, 0x89, 0xd9, 0xff, 0x54, 0x24, 0x10, 0x41, 0x89,
    0xc5, 0x44, 0x89, 0xe8, 0x48, 0x81, 0xc4, 0x98, 0x00, 0x00, 0x00, 0x5b, 0x41, 0x5c, 0x41, 0x5d,
    0x41, 0x5e, 0x41, 0x5f, 0x5d, 0xc3
}};

static const uint8_t INFINITE_LOOP[]{0xeb, 0xfe};

// NOLINTEND(*)

struct ExtraStuff {
	int sock;
	uintptr_t inf_loop; // haha open prospero go brrrrrrr
	uintptr_t func;
	uintptr_t socket;
	uintptr_t close;
	uintptr_t connect;
	uintptr_t send;
	uintptr_t recv;
	uintptr_t notifi;
	//uintptr_t syscore_printf;

	ExtraStuff(Hijacker &hijacker, uintptr_t loop) noexcept :
		sock(-1), inf_loop(loop), func(0),
		socket(hijacker.getLibKernelAddress(nid::socket)),
		close(hijacker.getLibKernelAddress(nid::close)),
		connect(hijacker.getLibKernelAddress(nid::connect)),
		send(hijacker.getLibKernelAddress(nid::send)),
		recv(hijacker.getLibKernelAddress(nid::recv)),
		//syscore_printf(hijacker.getFunctionAddress(hijacker.getLib(2).get(), nid::printf)) {}
		notifi(hijacker.getLibKernelAddress(nid::sceKernelSendNotificationRequest)) {}
};

UniquePtr<Hijacker> patchSyscore() {
	puts("patching syscore execve");
	auto hijacker = Hijacker::getHijacker("SceSysCore.elf"_sv);
	if (hijacker == nullptr) {
		puts("failed to get SceSysCore.elf");
		return nullptr;
	}

	uintptr_t code = hijacker->getTextAllocator().allocate(ShellcodeBuilder::SHELLCODE_SIZE);
	printf("shellcode addr: 0x%llx\n", code);
	uintptr_t loop = hijacker->getTextAllocator().allocate(sizeof(INFINITE_LOOP));
	printf("loop addr: 0x%llx\n", loop);
	hijacker->write(loop, INFINITE_LOOP);
	//static constexpr Nid printfNid{"hcuQgD53UxM"};
	//static constexpr Nid amd64_set_fsbaseNid{"3SVaehJvYFk"};
	auto rfork_thread = hijacker->getLibKernelFunctionAddress(nid::rfork_thread);
	printf("rfork_thread addr: 0x%llx\n", rfork_thread);
	if (rfork_thread == 0) {
		puts("failed to locate rfork_thread");
		return nullptr;
	}
	auto stuffAddr = hijacker->getDataAllocator().allocate(sizeof(ExtraStuff));
	printf("stuffAddr addr: 0x%llx\n", stuffAddr);
	auto meta = hijacker->getEboot()->getMetaData();
	const auto &plttab = meta->getPltTable();
	auto index = meta->getSymbolTable().getSymbolIndex(nid::rfork_thread);
	if (index == -1) {
		puts("rfork_thread import not found");
		return nullptr;
	}
	for (const auto &plt : plttab) {
		if (ELF64_R_SYM(plt.r_info) == index) {
			ShellcodeBuilder builder = BUILDER_TEMPLATE;
			ExtraStuff stuff{*hijacker, loop};
			builder.setRforkThreadAddr(rfork_thread);
			builder.setExtraStuffAddr(stuffAddr);
			hijacker->write(code, builder.shellcode);
			hijacker->write(stuffAddr, stuff);

			uintptr_t addr = hijacker->getEboot()->imagebase() + plt.r_offset;

			// write the hook
			hijacker->write<uintptr_t>(addr, code);
			return hijacker.release();
		}
	}
	return nullptr;
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
			return (void*)hijacker->getFunctionAddress(lib.get(), nid); // NOLINT(*)
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
			return (void*)hijacker->getFunctionAddress(lib.get(), nid); // NOLINT(*)
		}
	}
	return nullptr;
}

struct LaunchArgs {
	const char *titleId;
	uint32_t id;
	int *appId;
};

static bool launchApp(const char *titleId, int *appId) {
	puts("launching app");
	uint32_t id = -1;
	uint32_t libUserService = getLibUserService();
	printf("libUserService 0x%08lx\n", libUserService);
	if (libUserService == 0) {
		return false;
	}
	static constexpr auto DEFAULT_PRIORITY = 256;
	int priority = DEFAULT_PRIORITY;
	uint32_t (*sceUserServiceInitialize)(int*) = nullptr;
	sceKernelDlsym(libUserService, "sceUserServiceInitialize", reinterpret_cast<void**>(&sceUserServiceInitialize));
	if (sceUserServiceInitialize == nullptr) {
		puts("failed to resolve sceUserServiceInitialize");
		return false;
	}
	sceUserServiceInitialize(&priority);
	uint32_t (*sceUserServiceGetForegroundUser)(uint32_t *) = nullptr;
	sceKernelDlsym(libUserService, "sceUserServiceGetForegroundUser", reinterpret_cast<void**>(&sceUserServiceGetForegroundUser));
	if (sceUserServiceGetForegroundUser == nullptr) {
		puts("failed to resolve sceUserServiceGetForegroundUser");
		return false;
	}
	uint32_t res = sceUserServiceGetForegroundUser(&id);
	if (res != 0) {
		printf("sceUserServiceGetForegroundUser failed: 0x%llx\n", res);
		return false;
	}
	printf("user id %u\n", id);

	uint32_t libSystemService = getLibSystemService();
	printf("libSystemService 0x%08lx\n", libSystemService);

	// the thread will clean this up
	Flag flag = Flag_None;
	LncAppParam param{sizeof(LncAppParam), id, 0, 0, flag};
	using ftype = int (*)(const char* tid, const char* argv[], LncAppParam* param);
	int (*sceLncUtilLaunchApp)(const char* tid, const char* argv[], LncAppParam* param) = reinterpret_cast<ftype>(getSceLncUtilLaunchApp());
	if (sceLncUtilLaunchApp == nullptr) {
		puts("failed to get address of sceLncUtilLaunchApp");
		return false;
	}
	puts("calling sceLncUtilLaunchApp");
	int err = sceLncUtilLaunchApp(titleId, nullptr, &param);
	*appId = err;
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


static constexpr uintptr_t ENTRYPOINT_OFFSET = 0x70;

struct LoopBuilder {
	static constexpr size_t LOOB_BUILDER_SIZE = 39;
	static constexpr size_t LOOP_BUILDER_TARGET_OFFSET = 2;
	uint8_t data[LOOB_BUILDER_SIZE];

	void setTarget(uintptr_t addr) {
		*reinterpret_cast<uintptr_t *>(data + LOOP_BUILDER_TARGET_OFFSET) = addr;
	}
};

static inline constexpr LoopBuilder SLEEP_LOOP{
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

extern "C" int _write(int fd, const void *, size_t);

bool makeHomebrewApp();

struct Helper {
	uintptr_t nanosleepOffset;
	UniquePtr<Hijacker> spawned;
};


bool touch_file(const char* destfile) {
	static constexpr int FLAGS = 0777;
	int fd = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, FLAGS);
	if (fd > 0) {
		close(fd);
		return true;
	}
	return false;
}


int networkListen(const char* soc_path) {
	int s = socket(AF_UNIX, SOCK_STREAM, 0);
	if (s < 0) {
		printf("[Spawner] Socket failed! %s\n", strerror(errno));
		return -1;
	}

	struct sockaddr_un server{};
	server.sun_family = AF_UNIX;
	strncpy(server.sun_path, soc_path, sizeof(server.sun_path) - 1);

	int r = bind(s, reinterpret_cast<struct sockaddr *>(&server), SUN_LEN(&server));
	if (r < 0) {
		printf("[Spawner] Bind failed! %s Path %s\n", strerror(errno), server.sun_path);
		return -1;
	}

	printf("Socket has name %s\n", server.sun_path);

	r = listen(s, 1);
	if (r < 0) {
		printf("[Spawner] listen failed! %s\n", strerror(errno));
		return -1;
	}

	printf("touching %s\n", "/system_tmp/IPC");
    touch_file("/system_tmp/IPC");
	printf("network listen unix socket %d\n", s);
	return s;
}

class UnixSocket : public FileDescriptor {
	const char *path;

	public:
		UnixSocket(const char *path) noexcept : FileDescriptor(networkListen(path)), path(path) {}
		UnixSocket(const UnixSocket&) = delete;
		UnixSocket(UnixSocket &&rhs) noexcept = default;
		UnixSocket &operator=(const UnixSocket &rhs) = delete;
		UnixSocket &operator=(UnixSocket &&rhs) noexcept {
			FileDescriptor::operator=((FileDescriptor&&)rhs);
			path = rhs.path;
			return *this;
		}
		~UnixSocket() {
			close();
			unlink(path);
		}
};


static void *hookThread(void *args) noexcept {
	static constexpr int PING =  0;
	static constexpr int PONG =  1;
	static constexpr int PROCESS_LAUNCHED = 1;

	Helper *helper = reinterpret_cast<Helper *>(args);
	printf("hook thread started\n");
	auto hijacker = Hijacker::getHijacker(getppid());


	UnixSocket serverSock{"/system_tmp/IPC"};
	if (serverSock == -1) {
		printf("networkListen %i\n", (int)serverSock);
		return nullptr;
	}

	printf("listen done\n");

	FileDescriptor fd = accept(serverSock, nullptr, nullptr);
	if (fd == -1) {
		puts("accept failed");
		return nullptr;
	}

	puts("cli accepted");

	struct result {
		int cmd;
		int pid;
		uintptr_t func;
	} res{};


	if (_read(fd, &res, sizeof(res)) == -1) {
		puts("read failed");
		return nullptr;
	}

	if (res.cmd == PING) {
		puts("ping received");
		int reply = PONG;
		if (_write(fd, &reply, sizeof(reply)) == -1) {
			puts("write failed");
			return nullptr;
		}
		if (_read(fd, &res, sizeof(res)) == -1) {
			puts("read failed");
			return nullptr;
		}
	}

	if (res.cmd != PROCESS_LAUNCHED) {
		puts("not launched");
		return nullptr;
	}

	puts("next");

	// close it so it can be opened in the spawned daemon
	fd.close();

	LoopBuilder loop = SLEEP_LOOP;
	const int pid = res.pid;

	dbg::Tracer tracer{pid};
	auto regs = tracer.getRegisters();
	regs.rip(res.func);
	tracer.setRegisters(regs);

	puts("running until execve completes");

	// run until execve completion
	tracer.run();

	puts("execve completed");

	helper->spawned = Hijacker::getHijacker(pid);

	uintptr_t base = helper->spawned->getLibKernelBase();

	printf("libkernel imagebase: 0x%08llx\n", base);

	puts("spawned process obtained");

	puts("success");

	loop.setTarget(base + helper->nanosleepOffset);
	base = helper->spawned->imagebase();

	// force the entrypoint to an infinite loop so that it doesn't start until we're ready
	dbg::write(pid, base + ENTRYPOINT_OFFSET, loop.data, sizeof(loop.data));

	puts("finished");
	printf("spawned imagebase 0x%08llx\n", base);

	return nullptr;
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

	if (!makeHomebrewApp()) {
		return 0;
	}

	uint8_t qaflags[QAFLAGS_SIZE];
	kread<QAFLAGS_SIZE>(kernel_base + offsets::qa_flags(), qaflags);
	qaflags[1] |= 1 | 2;
	kwrite<QAFLAGS_SIZE>(kernel_base + offsets::qa_flags(), qaflags);

	auto syscore = patchSyscore();
	const uintptr_t nanosleepOffset = getNanosleepOffset(*syscore);
	//puts("spawning daemon");

	Helper helper{nanosleepOffset, nullptr};
	pthread_t td = nullptr;
	pthread_create(&td, nullptr, hookThread, &helper);

	usleep(100000); // NOLINT(*)

	int appId = 0;
	if (!launchApp("BREW00000", &appId)) {
		// we're screwed
		return 0;
	}

	// the thread should have already completed
	pthread_join(td, nullptr);

	if (helper.spawned == nullptr || !load(helper.spawned)) {
		puts("failed to load elf into new process");
		using ftype = uint32_t (*)(uint32_t);
		auto sceLncUtilKillApp = (ftype) getSceLncUtilKillApp(); // NOLINT(*)
		sceLncUtilKillApp(appId);
		return 0;
	}

	return 0;
}

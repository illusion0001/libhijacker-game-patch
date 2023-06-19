
typedef unsigned long long u64;
typedef long long s64;
typedef unsigned int u32;
typedef int s32;

constexpr u32 PROT_READ = 1;
constexpr u32 PROT_WRITE = 2;
constexpr u32 PROT_EXEC = 4;

constexpr u32 MAP_SHARED   = 0x1;
constexpr u32 MAP_PRIVATE   = 0x2;
constexpr u32 MAP_FIXED     = 0x10;
constexpr u32 MAP_ANONYMOUS = 0x1000;

constexpr s64 MAP_FAILED = -1;

struct Info {
	u64 paddr;
	u64 length;
	s64 result;
	u32 protection;
};

struct Args {
	struct Result {
		s32 state;
		s32 err;
	} result;
	int (*usleep)(int);
	s64 (*mmap)(u64 addr, u64 len, u32 prot, u32 flags, int fd, u64 offset);
	int (*munmap)(u64 addr, u64 len);
	int (*sceKernelJitCreateSharedMemory)(void *addr, u64 length, u32 flags, int *p_fd);
	int *(*errno)();
	Info *__restrict info;
	int numInfo;
};


[[noreturn]] void shellcode(Args *__restrict args) {

	u64 totalSize = 0;

	for (int i = 0; i < args->numInfo; i++) {
		totalSize += args->info[i].length;
	}

	// should help ensure we get a contiguous range
	s64 mem = args->mmap(0, totalSize, PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (mem == -1) [[unlikely]] {
		args->result = {2, *args->errno()};
		while (true) {
			args->usleep(1000000);
		}
	}

	args->munmap(mem, totalSize);

	const auto length = args->info[0].length;

	int fd = -1;
	int res = args->sceKernelJitCreateSharedMemory(nullptr, length, PROT_READ|PROT_WRITE|PROT_EXEC, &fd);
	if (res != 0) [[unlikely]] {
		args->result = {3, res};
		while (true) {
			args->usleep(1000000);
		}
	}

	// it's very picky with what it allows for jit
	const auto imagebase = args->mmap(mem, length, PROT_EXEC, MAP_FIXED | MAP_SHARED, fd, 0);
	args->info[0].result = imagebase;
	if (imagebase == MAP_FAILED) [[unlikely]] {
		args->result = {4, *args->errno()};
		while (true) {
			args->usleep(1000000);
		}
	}

	for (int i = 1; i < args->numInfo; i++) {
		// ensure they are contiguous in loader
		const u64 length = args->info[i].length;
		const u64 addr = args->info[i].paddr + imagebase;
		auto mmapResult = args->mmap(addr, length, args->info[i].protection, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		args->info[i].result = mmapResult;
		if (mmapResult == MAP_FAILED) [[unlikely]] {
			args->result = {5, *args->errno()};
			while (true) {
				args->usleep(1000000);
			}
		}
	}

	args->result.state = 1;

	while (true) {
		args->usleep(1000000);
	}
}

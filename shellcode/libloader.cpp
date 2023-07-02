
typedef unsigned long long u64;
typedef unsigned int u32;
typedef int s32;

static constexpr u32 INTERNAL_MASK = 0x80000000;

struct Args {
	struct Result {
		s32 state;
		s32 err;
	} result;
	int (*sceSysmoduleLoadModuleInternal)(u32);
	int (*sceSysmoduleLoadModuleByNameInternal)(const char *fname, u64, u64, u64, u64, u64);
	int (*usleep)(int);
	const char *__restrict strtab;
	u64 *__restrict offsets;
	int numOffsets;
};

[[noreturn]] void shellcode(Args *__restrict args) {

	for (int i = 0; i < args->numOffsets; i++) {
		int handle;
		const auto offset = args->offsets[i];
		if ((offset & INTERNAL_MASK) != 0) {
			handle = args->sceSysmoduleLoadModuleInternal(offset);
		} else {
			handle = args->sceSysmoduleLoadModuleByNameInternal(args->strtab + offset, 0, 0, 0, 0, 0);
		}
		if (handle < 0) [[unlikely]] {
			args->result = {2, handle};
			while (true) {
				args->usleep(1000000);
			}
		}
		args->offsets[i] = handle;
	}

	args->result.state = 1;

	while (true) {
		args->usleep(1000000);
	}
}


typedef unsigned long long u64;
typedef int s32;

struct Args {
	struct Result {
		s32 state;
		s32 err;
	} result;
	int (*sceSysmoduleLoadModuleByNameInternal)(const char *fname, u64, u64, u64, u64, u64);
	int (*usleep)(int);
	const char *__restrict strtab;
	u64 *__restrict offsets;
	int numOffsets;
};

[[noreturn]] void shellcode(Args *__restrict args) {

	for (int i = 0; i < args->numOffsets; i++) {
		int handle = args->sceSysmoduleLoadModuleByNameInternal(args->strtab + args->offsets[i], 0, 0, 0, 0, 0);
		if (handle < 1) [[unlikely]] {
			args->result = {2, i};
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

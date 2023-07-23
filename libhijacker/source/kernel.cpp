#include "kernel.hpp"
#include "util.hpp"

extern "C" {
	#include <ps5/kernel.h>
}

static constexpr size_t BUF_SIZE = 0x10;

String getKernelString(uintptr_t addr) {
	String res{};
	char buf[BUF_SIZE];
	while (true) {
		kernel_copyout(addr + res.length(), buf, sizeof(buf));
		size_t read = strnlen(buf, sizeof(buf));
		res += StringView{buf, read};
		if (read < sizeof(buf)) {
			return res;
		}
	}
}

UniquePtr<KProc> getProc(int pid) {
	for (auto p : getAllProcs()) {
		if (pid == p->pid()) {
			return p.release();
		}
	}
	return nullptr;
}

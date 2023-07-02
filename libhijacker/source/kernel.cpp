#include "kernel.hpp"
#include "util.hpp"

extern "C" {
	#include <ps5/kernel.h>
}

String getKernelString(uintptr_t addr) {
	String res{};
	char buf[0x10];
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

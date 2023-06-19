#pragma once
#include "kernel.hpp"
#include "frame.hpp"

class KThread : public KernelObject<KThread, 0x680> {

	public:
		KThread(uintptr_t addr) : KernelObject(addr) {}

		UniquePtr<TrapFrame> getFrame() {
			uintptr_t ptr = get<uintptr_t, 0x460>();
			return ptr ? new TrapFrame(ptr) : nullptr;
		}

		int tid() const {
			return get<int, 0x9c>();
		}
};

/*
UniquePtr<KThread> getThread(int pid, int tid) {
	KThread
}
*/

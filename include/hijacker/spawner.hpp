#pragma once

#include "hijacker.hpp"
#include "memory.hpp"
#include "util.hpp"

extern "C" {
	#include <stdint.h>
}



class Spawner {
	dbg::IdArray pids;
	ProcessPointer<int32_t> state;
	UniquePtr<Hijacker> hijacker;
	uintptr_t entry;
	uintptr_t dlsym;
	uintptr_t nanosleepOffset;
	uintptr_t argbuf;
	int pid;

	Spawner(Hijacker *ptr);
	int32_t getResult() const;

	public:
		~Spawner() { *state = false; }
		static UniquePtr<Spawner> getSpawner(const StringView &processName) {
			auto hijacker = Hijacker::getHijacker(processName);
			return hijacker ? new Spawner(hijacker.release()) : nullptr;
		}
		static UniquePtr<Spawner> getSpawner(int pid) {
			return new Spawner(Hijacker::getHijacker(pid).release());
		}
		Hijacker *getHijacker() const { return hijacker.get(); }
		UniquePtr<Hijacker> spawn();
};

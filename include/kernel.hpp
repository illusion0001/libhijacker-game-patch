#pragma once

#include "kernel.hpp"
#include "kernel/kernel.hpp"
#include "kernel/rtld.hpp"
#include "kernel/proc.hpp"
#include "kernel/frame.hpp"
#include "util.hpp"

bool createReadWriteSockets(const UniquePtr<KProc> &proc, const int *sockets) noexcept;

inline bool createReadWriteSockets(int pid, const int *sockets) noexcept {
	auto proc = getProc(pid);
	if (proc == nullptr) {
		puts("createReadWriteSockets proc == nullptr");
		return false;
	}
	return createReadWriteSockets(proc, sockets);
}

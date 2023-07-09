#pragma once

#include "kernel.hpp"
#include "kernel/kernel.hpp"
#include "rtld.hpp"
#include "kthread.hpp"
#include "../offsets.hpp"
#include "util.hpp"

extern "C" {
	#include <ps5/kernel.h>
	extern int getpid();
	extern uintptr_t kernel_base;
}

struct SelfInfo {
	uint32_t type;
	uint32_t ps4_sdk_version;
	uint32_t ps5_sdk_version;
	uint32_t beef_face;
	uint32_t dbbcc;
	char name[32];
	char path[972];
};

static_assert(sizeof(SelfInfo) == 0x400, "sizeof(SelfInfo) != 0x400");

class Filedescent : public KernelObject<Filedescent, 0x30> {

	public:
		Filedescent(uintptr_t addr) : KernelObject(addr) {}
		uintptr_t file() const {
			return get<uintptr_t, 0>();
		}

		uint8_t flags() const {
			return get<uint8_t, 0x28>();
		}

		uint32_t seq() const {
			return get<uint32_t, 0x2c>();
		}
};

class FdTbl {
	uintptr_t addr;
	size_t ntables;

	size_t checkIndex(size_t i) const {
		#ifdef DEBUG
		if (i >= ntables) [[unlikely]] {
			fatalf("index %llu is out of bounds for length %llu\n", i, ntables);
		}
		#endif
		return i;
	}

	public:
		FdTbl(uintptr_t addr) : addr(addr), ntables(kread<uintptr_t>(addr) & 0xffffffff) {}
		FdTbl(const FdTbl&) = default;
		FdTbl &operator=(const FdTbl&) = default;
		FdTbl(FdTbl&&) = default;
		FdTbl &operator=(FdTbl&&) = default;

		Filedescent operator[](size_t i) const {
			return {addr + (checkIndex(i) * Filedescent::length) + sizeof(ntables)};
		}

		uintptr_t getFile(size_t i) const {
			uintptr_t fp = addr + (checkIndex(i) * Filedescent::length) + sizeof(ntables);
			return kread<uintptr_t>(fp);
		}

		void setFile(size_t i, uintptr_t file) const {
			uintptr_t fp = addr + (checkIndex(i) * Filedescent::length) + sizeof(ntables);
			kwrite(fp, file);
		}

		uintptr_t getFileData(size_t i) const {
			return kread<uintptr_t>(getFile(i));
		}

		void setFileData(size_t i, uintptr_t data) const {
			kwrite(getFile(i), data);
		}

		size_t length() const {
			return ntables;
		}
};

class KProc : public KernelObject<KProc, 0xc90> {

	public:

		KProc(uintptr_t addr) : KernelObject(addr) {}
		uintptr_t p_ucred() const {
			return get<uintptr_t, 0x40>();
		}

		UniquePtr<KUcred> ucred() const {
			return new KUcred{p_ucred()};
		}

		int p_pid() const {
			return get<int, 0xbc>();
		}

		int pid() const {
			return p_pid();
		}

		UniquePtr<SharedObject> getSharedObject() const {
			auto obj = get<uintptr_t, 0x3e8>();
			return obj != 0 ? new SharedObject{obj, pid()} : nullptr;
		}

		KIterator<KThread> p_threads() const {
			return address() + 0x10;
		}

		KIterator<KThread> getThreads() const {
			return p_threads();
		}

		UniquePtr<KThread> getThread(int tid) const {
			for (auto td : getThreads()) {
				if (tid == td->tid()) {
					return td.release();
				}
			}
			return nullptr;
		}

		uintptr_t p_fd() const {
			return get<uintptr_t, 0x48>();
		}

		FdTbl getFdTbl() const {
			return {kread<uintptr_t>(p_fd())};
		}

		const SelfInfo *getSelfInfo() const {
			return (SelfInfo *) (buf + 0x588);
		}

		// no flush required
		void setName(const StringView &name, bool reload=false) {
			const size_t length = name.length() < 0x1f ? name.length() : 0x20;
			kernel_copyin(const_cast<char*>(name.c_str()), address() + 0x59C, length);
			if (reload) {
				this->reload();
			}
		}
};

inline KIterator<KProc> getAllProcs() {
	return {kernel_base + offsets::allproc()};
}

UniquePtr<KProc> getProc(int pid);

inline UniquePtr<KProc> getProc() {
	return getProc(getpid());
}

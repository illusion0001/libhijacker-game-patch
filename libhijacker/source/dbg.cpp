#include "dbg.hpp"
#include "kernel.hpp"
#include "util.hpp"

extern "C" {
#include <ps5/kernel.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
}

static constexpr uint64_t DEBUGGER_AUTHID = 0x4800000000000006;
typedef void (*p_mdbg_call)(void *, void *, void *);
extern "C" p_mdbg_call _mdbg = nullptr;

extern "C" int sceKernelDlsym(int handle, const char* symbol, void** addrp);
extern "C" int *__error();

static constexpr uintptr_t PID_OFFSET = 0xBC;
static constexpr uintptr_t UCRED_OFFSET = 0x40;

int __attribute__((naked, noinline)) syscall_mdbg_call(void *arg1, void *arg2, void *arg3) {
	asm (
		"mov $573, %rax\n"
		"call *_mdbg(%rip)\n"
		"ret\n"
	);
}

static uintptr_t getCurrentProc() {

	pid_t pid = getpid();

	/*
	uintptr_t pidhashtbl = kread<uintptr_t>(kernel_base + OFFSET_KERNEL_PID_HASHTBL);
	uintptr_t proc = kread<uintptr_t>(pidhashtbl + ((pid & 0xff) * 8));
	while (proc != 0) {
		int cid = kread<int>(proc + OFFSET_KERNEL_PROC_P_PID);
		if (cid == pid) {
			return proc;
		}
		proc = kread<uintptr_t>(proc);
	}
	*/

	uintptr_t proc = kread<uintptr_t>(kernel_base + offsets::allproc());
	while (proc != 0) {
		int cid = kread<int>(proc + PID_OFFSET);
		if (cid == pid) {
			return proc;
		}
		proc = kread<uintptr_t>(proc);
	}
	return 0;
}

namespace dbg {

class DbgAuthidSwapper {

	uint64_t id;
	static constexpr int AUTHID_OFFSET = 0x58;

public:
	DbgAuthidSwapper(uint64_t authid) {
		uintptr_t proc = getCurrentProc();
		uintptr_t ucred = kread<uintptr_t>(proc + UCRED_OFFSET);
		id = kread<uint64_t>(ucred + AUTHID_OFFSET);
		kwrite(ucred + AUTHID_OFFSET, authid);
	}
	~DbgAuthidSwapper() {
		uintptr_t proc = getCurrentProc();
		uintptr_t ucred = kread<uintptr_t>(proc + UCRED_OFFSET);
		kwrite(ucred + AUTHID_OFFSET, id);
	}
};

int __attribute__((noinline)) mdbg_call(DbgArg1 &arg1, DbgArg2 &arg2, DbgArg3 &arg3) {

	if (!_mdbg) [[unlikely]] {
		uint64_t addr = 0;
		int res = sceKernelDlsym(0x2001, "get_authinfo", (void **) &addr);
		if (res > -1 && addr) {
			_mdbg = (p_mdbg_call)(addr + 7);
		} else {
			puts("failed to get get_authinfo for mdbg_call");
		}
	}

	if (_mdbg) [[likely]] {
		DbgAuthidSwapper swapper{DEBUGGER_AUTHID};
		return syscall_mdbg_call(&arg1, &arg2, &arg3);
	}
	puts("_mdbg is null");
	return -1;
}

IdArray getAllPids() {
	DbgArg1 arg1{1, DbgCommand::PROCESS_LIST_CMD};
	static constexpr size_t length = 10000;
	UniquePtr<int[]> buf{new int[length]};
	DbgGetPidsArg arg2{buf.get(), length};
	DbgArg3 arg3{};
	mdbg_call(arg1, arg2, arg3);
	return {buf.get(), arg3.length};
}

IdArray getAllTids(int pid) {
	DbgArg1 arg1{1, DbgCommand::THREAD_LIST_CMD};
	static constexpr size_t length = 0x2000;
	UniquePtr<int[]> buf{new int[length]};
	DbgGetTidsArg arg2{pid, buf.get(), length};
	DbgArg3 arg3{};
	mdbg_call(arg1, arg2, arg3);
	return {buf.get(), arg3.length};
}

void ProcessInfo::fillInfo() {
	DbgArg1 arg1{1, DbgCommand::PROCESS_INFO_CMD};
	DbgGetProcInfoArg arg2{_pid, buf.get(), BUF_LENGTH};
	DbgArg3 arg3{};
	mdbg_call(arg1, arg2, arg3);

}

void ThreadInfo::fillInfo() {
	DbgArg1 arg1{1, DbgCommand::THREAD_INFO_CMD};
	DbgGetThreadInfoArg arg2{_pid, _tid, buf.get(), BUF_LENGTH};
	DbgArg3 arg3{};
	mdbg_call(arg1, arg2, arg3);
}

void suspend(int pid) {
	DbgArg1 arg1{1, DbgCommand::ARG2_CMD};
	DbgKickProcessArg arg2{pid};
	DbgArg3 arg3{};
	mdbg_call(arg1, arg2, arg3);
}

void resume(int pid) {
	// this is the same as suspend but is separate for easier debugging
	DbgArg1 arg1{1, DbgCommand::ARG2_CMD};
	DbgKickProcessArg arg2{pid};
	DbgArg3 arg3{};
	mdbg_call(arg1, arg2, arg3);
}

void read(int pid, uintptr_t src, void *dst, size_t length) {
	DbgArg1 arg1{1, DbgCommand::READ_CMD};
	DbgReadArg arg2{pid, src, dst, length};
	DbgArg3 arg3{};
	mdbg_call(arg1, arg2, arg3);
	if (arg3.length != length) {
		int err = arg3.err != -1 ? (int) arg3.err : errno;
		printf("read failed %d: %s\n", err, strerror(err));
	}
}

bool write(int pid, uintptr_t dst, const void *src, size_t length) {
	DbgArg1 arg1{1, DbgCommand::WRITE_CMD};
	DbgReadArg arg2{pid, dst, const_cast<void *>(src), length};
	DbgArg3 arg3{};
	mdbg_call(arg1, arg2, arg3);
	if (arg3.length != length) {
		int err = arg3.err != -1 ? (int) arg3.err : errno;
		printf("write failed %d: %s\n", err, strerror(err));
		return false;
	}
	return true;
}


} // mdbg

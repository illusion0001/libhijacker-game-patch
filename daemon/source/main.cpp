#include <netinet/in.h>
#include <poll.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "fd.hpp"
#include "hijacker/hijacker.hpp"
#include "servers.hpp"
#include "util.hpp"

extern int runKlogger(void *unused);
extern int runElfServer(void *unused);
extern int runCommandProcessor(void *unused);
extern void makenewapp();

void AbortServer::run(TcpSocket &sock) {
	// any connection signals to shutdown the daemon
	puts("abort signal received");
	sock.close();
}

struct HookBuilder {
	static constexpr size_t CODE_SIZE = 92;
	//static constexpr size_t PRINTF_OFFSET = 0x31;
	//static constexpr size_t EXECVE_OFFSET = 0x4F;
	static constexpr size_t EXECVE_OFFSET = 0x4D;
	uint8_t data[CODE_SIZE];

	void setPrintf(uintptr_t addr) {
		//*reinterpret_cast<uintptr_t *>(data + PRINTF_OFFSET) = addr;
	}
	void setExecve(uintptr_t addr) {
		*reinterpret_cast<uintptr_t *>(data + EXECVE_OFFSET) = addr;
	}
};

// NOLINTBEGIN(*)

/*
int hookExecve(const char *path, const char **argv, const char **envp) {
	printf("execve %s\n", path);
	return execve(path, argv, envp);
}
*/
/*
static constexpr HookBuilder HOOKER{{
    0x48, 0x83, 0xec, 0x28, 0x48, 0xb8, 0x2f, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x5f, 0x48, 0xbf,
    0x65, 0x78, 0x2f, 0x61, 0x70, 0x70, 0x2f, 0x4e, 0x48, 0xb9, 0x50, 0x58, 0x53, 0x34, 0x30, 0x30,
    0x32, 0x38, 0x48, 0x89, 0x04, 0x24, 0x48, 0x89, 0x7c, 0x24, 0x08, 0x48, 0x89, 0xe7, 0x48, 0xb8,
    0x2f, 0x65, 0x62, 0x6f, 0x6f, 0x74, 0x2e, 0x62, 0x48, 0x89, 0x4c, 0x24, 0x10, 0x48, 0x89, 0x44,
    0x24, 0x18, 0x48, 0xc7, 0x44, 0x24, 0x20, 0x69, 0x6e, 0x00, 0x00, 0x48, 0xb8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x48, 0x83, 0xc4, 0x28, 0xc3
}};

static constexpr HookBuilder HOOKER{{
	// PUSH R15
	0x41, 0x57,
	// PUSH R14
	0x41, 0x56,
	// PUSH RBX
	0x53,
	// SUB RSP, 0x10
	0x48, 0x83, 0xec, 0x10,
	// MOV R15, RDI
	0x49, 0x89, 0xff,
	// MOV RAX, 0x2520657663657865
	0x48, 0xb8, 0x65, 0x78, 0x65, 0x63, 0x76, 0x65, 0x20, 0x25,
	// MOV R14, RSI
	0x49, 0x89, 0xf6,
	// MOV RDI, RSP
	0x48, 0x89, 0xe7,
	// MOV RBX, RDX
	0x48, 0x89, 0xd3,
	// MOV qword ptr [RSP], RAX
	0x48, 0x89, 0x04, 0x24,
	// MOV RSI, R15
	0x4c, 0x89, 0xfe,
	// MOV qword ptr [RSP + 0x8], 0x6e5c73
	0x48, 0xc7, 0x44, 0x24, 0x08, 0x73, 0x5c, 0x6e, 0x00,
	// MOV RAX, printf
	0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	// CALL RAX
	0xff, 0xd0,
	// MOV RDI, R15
	0x4c, 0x89, 0xff,
	// MOV RSI, R14
	0x4c, 0x89, 0xf6,
	// MOV RDX, RBX
	0x48, 0x89, 0xda,
	// ADD RSP, 0x10
	0x48, 0x83, 0xc4, 0x10,
	// POP RBX
	0x5b,
	// POP R14
	0x41, 0x5e,
	// POP R15
	0x41, 0x5f,
	// MOV RAX, execve
	0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	// JMP RAX
	0xff, 0xe0
}};
*/

// NOLINTEND(*)
/*
bool patchSyscore() {
	return true;
	puts("patching syscore execve");
	auto hijacker = Hijacker::getHijacker("SceSysCore.elf"_sv);
	if (hijacker == nullptr) {
		puts("failed to get SceSysCore.elf");
		return false;
	}
	uintptr_t code = hijacker->getTextAllocator().allocate(HookBuilder::CODE_SIZE);
	static constexpr Nid execveNid{"kdguLiAheLI"};
	//static constexpr Nid printfNid{"hcuQgD53UxM"};
	//static constexpr Nid amd64_set_fsbaseNid{"3SVaehJvYFk"};
	auto execve = hijacker->getLibKernelFunctionAddress(execveNid);
	if (execve == 0) {
		puts("failed to locate execve");
		return false;
	}
	auto meta = hijacker->getEboot()->getMetaData();
	const auto &plttab = meta->getPltTable();
	if (ELF64_R_TYPE(plttab[0].r_info) != R_X86_64_JMP_SLOT) {
		puts("wrong table stupid");
		return false;
	}
	auto index = meta->getSymbolTable().getSymbolIndex(execveNid);
	if (index == -1) {
		puts("execve import not found");
		return false;
	}
	*/
	/*
	auto fsbaseIndex = meta->getSymbolTable().getSymbolIndex(amd64_set_fsbaseNid);
	for (const auto &plt : plttab) {
		if (ELF64_R_SYM(plt.r_info) == fsbaseIndex) {
			auto retptr = hijacker->getTextAllocator().allocate(1);
			uint8_t ret[]{0xc3}; // NOLINT(*)
			hijacker->write(retptr, ret);
			uintptr_t addr = hijacker->getEboot()->imagebase() + plt.r_offset;
			hijacker->write<uintptr_t>(addr, retptr);
			break;
		}
	}

	for (const auto &plt : plttab) {
		if (ELF64_R_SYM(plt.r_info) == index) {
			HookBuilder hooker = HOOKER;
			auto printfAddr = hijacker->getFunctionAddress(hijacker->getLib(2).get(), printfNid);
			if (printfAddr == 0) {
				puts("failed to locate printf");
				return false;
			}
			hooker.setPrintf(printfAddr);
			hooker.setExecve(execve);
			hijacker->write(code, hooker.data);
			uintptr_t addr = hijacker->getEboot()->imagebase() + plt.r_offset;
			hijacker->write<uintptr_t>(addr, code);
			hexdump(hooker.data, HookBuilder::CODE_SIZE);
			return true;
		}
	}
	return false;
}*/

int main() {
	puts("daemon entered");
	AbortServer abortServer{};
	KlogServer klogServer{};
	ElfServer elfServer{};
	CommandServer commandServer{};

	abortServer.TcpServer::run();
	klogServer.TcpServer::run();
	elfServer.TcpServer::run();
	commandServer.TcpServer::run();

	/*if (!patchSyscore()) {
		puts("failed to patch syscore execve");
	} else {
		puts("successfully patched syscore execve");
	}*/

	// finishes on connect
	abortServer.join();
	puts("abort thread finished");
	commandServer.stop();
	puts("command server done");
	puts("stopping elf server");
	elfServer.stop();
	puts("elf server done");
	puts("stopping klog server");
	klogServer.stop();
	puts("klog server done");

	// TODO add elf loader with options for process name and type (daemon/game)
	// add whatever other crap people may want
	return 0;
}

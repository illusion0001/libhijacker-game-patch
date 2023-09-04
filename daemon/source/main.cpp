#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/_pthreadtypes.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>
#include "dbg.hpp"
#include "dbg/dbg.hpp"
#include "elf/elf.hpp"
#include "fd.hpp"
#include "hijacker/hijacker.hpp"
#include "servers.hpp"
#include "util.hpp"

extern int runKlogger(void *unused);
extern int runElfServer(void *unused);
extern "C" void free(void*);
extern int runCommandProcessor(void *unused);
extern void makenewapp();

extern "C" ssize_t _read(int, void *, size_t);
extern "C" ssize_t _write(int, void *, size_t);

void AbortServer::run(TcpSocket &sock) {
	// any connection signals to shutdown the daemon
	puts("abort signal received");
	sock.close();
}

static constexpr uintptr_t ENTRYPOINT_OFFSET = 0x70;

struct LoopBuilder {
	static constexpr size_t LOOB_BUILDER_SIZE = 39;
	static constexpr size_t LOOP_BUILDER_TARGET_OFFSET = 2;
	uint8_t data[LOOB_BUILDER_SIZE];

	void setTarget(uintptr_t addr) {
		*reinterpret_cast<uintptr_t *>(data + LOOP_BUILDER_TARGET_OFFSET) = addr;
	}
};

static inline constexpr LoopBuilder SLEEP_LOOP{
	// // 48 b8 xx xx xx xx xx xx xx xx 48 c7 c7 40 42 0f 00 ff d0 eb eb
	//loop:
	//	MOV RAX, _nanosleep
	0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	// MOV RDI, 1000000000 // 1 second
	0x48, 0xc7, 0xc7, 0x00, 0xca, 0x9a, 0x3b,
	// MOV RSI, 0
	0x48, 0xc7, 0xc6, 0x00, 0x00, 0x00, 0x00,
	// PUSH RDI
	0x57,
	// PUSH RSI
	0x56,
	// CALL RAX
	0xff, 0xd0,
	// JMP loop
	0xeb, 0xe2
};

static uintptr_t getNanosleepOffset(const Hijacker &hijacker) {
	uintptr_t addr = hijacker.getLibKernelFunctionAddress(nid::_nanosleep);
	return addr - hijacker.getLibKernelBase();
}

struct Helper {
	uintptr_t nanosleepOffset;
	UniquePtr<Hijacker> spawned;
};

static bool runElf(Hijacker *hijacker, uint8_t *data) {

	Elf elf{hijacker, data};

	if (!elf) {
		return false;
	}

	if (elf.launch()) {
		puts("launch succeeded");
		return true;
	}
	puts("launch failed");
	return false;
}

static bool load(UniquePtr<Hijacker> &spawned, uint8_t *data) {
	puts("setting process name");
	spawned->getProc()->setName("HomebrewDaemon"_sv);
	__builtin_printf("new process %s pid %d\n", spawned->getProc()->getSelfInfo()->name, spawned->getPid());
	puts("jailbreaking new process");
	spawned->jailbreak(false);

	// listen on a port for now. in the future embed the daemon and load directly

	if (runElf(spawned.get(), data)) {
		__builtin_printf("process name %s pid %d\n", spawned->getProc()->getSelfInfo()->name, spawned->getPid());
		return true;
	}
	return false;
}

// Function to read a file into memory using calloc
UniquePtr<uint8_t[]> readFileIntoBuffer(const char *filename) {
	struct stat st{};
	if (stat(filename, &st) == -1) {
		if (errno != ENOENT) {
			perror("stat failed");
		}
		return nullptr;
	}

    FileDescriptor fd = open(filename, O_RDONLY); // Open the file in binary mode
    if (fd == -1) {
        perror("Error opening file");
        return nullptr;
    }

	UniquePtr<uint8_t[]> buf{new uint8_t[st.st_size]};

	if (read(fd, buf.get(), st.st_size) == -1) {
		perror("read failed");
		return nullptr;
	}

	return buf;
}

bool touch_file(const char* destfile) {
	static constexpr int FLAGS = 0777;
	int fd = open(destfile, O_WRONLY | O_CREAT | O_TRUNC, FLAGS);
	if (fd > 0) {
		close(fd);
		return true;
	}
	return false;
}

int networkListen(const char* soc_path) {
	int s = socket(AF_UNIX, SOCK_STREAM, 0);
	if (s < 0) {
		printf("[Spawner] Socket failed! %s\n", strerror(errno));
		return -1;
	}

	struct sockaddr_un server{};
	server.sun_family = AF_UNIX;
	strncpy(server.sun_path, soc_path, sizeof(server.sun_path) - 1);

	int r = bind(s, reinterpret_cast<struct sockaddr *>(&server), SUN_LEN(&server));
	if (r < 0) {
		printf("[Spawner] Bind failed! %s Path %s\n", strerror(errno), server.sun_path);
		return -1;
	}

	printf("Socket has name %s\n", server.sun_path);

	r = listen(s, 1);
	if (r < 0) {
		printf("[Spawner] listen failed! %s\n", strerror(errno));
		return -1;
	}

	printf("touching %s\n", "/system_tmp/IPC");
    touch_file("/system_tmp/IPC");
	printf("network listen unix socket %d\n", s);
	return s;
}

extern "C" uint32_t _sceApplicationGetAppId(int pid, uint32_t *appId);
extern "C" uint32_t sceLncUtilKillApp(uint32_t appId);

static void killApp(int pid) noexcept {
	uint32_t appId = 0;
	_sceApplicationGetAppId(pid, &appId);
	if (appId != 0) {
		sceLncUtilKillApp(appId);
	}
}

static bool handleIpc(const int syscore, const int fd) noexcept {
	static constexpr int PING =  0;
	static constexpr int PONG =  1;
	static constexpr int PROCESS_LAUNCHED = 1;

	bool result = true;

	struct result {
		int cmd;
		int pid;
		uintptr_t func;
	} res{};

	if (recv(fd, &res, sizeof(res), MSG_NOSIGNAL) == -1) {
		printf("reading result failed\n");
		return result;
	}

	if (res.cmd == PING) {
		int reply = PONG;
		if (_write(fd, &reply, sizeof(reply)) == -1) {
			printf("writing pong failed\n");
			return result;
		}
		if (recv(fd, &res, sizeof(res), MSG_NOSIGNAL) == -1) {
			printf("reading result failed\n");
			return result;
		}
	}

	if (res.cmd != PROCESS_LAUNCHED) {
		printf("unexpected command %d\n", res.cmd);
		return result;
	}

	result = false;

	LoopBuilder loop = SLEEP_LOOP;
	const int pid = res.pid;

	UniquePtr<Hijacker> spawned = nullptr;
	{
		dbg::Tracer tracer{pid};
		auto regs = tracer.getRegisters();
		regs.rip(res.func);
		tracer.setRegisters(regs);

		// run until execve completion
		tracer.run();

		while (spawned == nullptr) {
			// this should grab it first try but I haven't confirmed yet
			spawned = Hijacker::getHijacker(pid);
		}

		const uintptr_t nanosleepOffset = getNanosleepOffset(*spawned);

		printf("libkernel imagebase: 0x%08llx\n", spawned->getLibKernelBase());

		puts("spawned process obtained");

		puts("success");

		uintptr_t base = 0;
		while (base == 0) {
			// this should also work first try but not confirmed
			base = spawned->getLibKernelBase();
		}

		loop.setTarget(base + nanosleepOffset);
		base = spawned->imagebase();

		// force the entrypoint to an infinite loop so that it doesn't start until we're ready
		dbg::write(pid, base + ENTRYPOINT_OFFSET, loop.data, sizeof(loop.data));

		puts("finished");
		printf("spawned imagebase 0x%08llx\n", base);
	}

	auto path = getProc(pid)->getPath();
	auto index = path.rfind('/');
	if (index == -1) {
		printf("path missing / : %s\n", path.c_str());
	} else {
		path = path.substring(0, index + 1) + "homebrew.elf";
		printf("loading elf %s\n", path.c_str());
	}
	auto data = readFileIntoBuffer(path.c_str());
	if (data == nullptr) {
		puts("failed to read elf");
		killApp(pid);
		return result;
	}

	if (load(spawned, data.get())) {
		puts("elf loaded");
	} else {
		puts("failed to load elf");
		killApp(pid);
	}
	return result;
}

class UnixSocket : public FileDescriptor {
	const char *path;

	public:
		UnixSocket(const char *path) noexcept : FileDescriptor(networkListen(path)), path(path) {}
		UnixSocket(const UnixSocket&) = delete;
		UnixSocket(UnixSocket &&rhs) noexcept = default;
		UnixSocket &operator=(const UnixSocket &rhs) = delete;
		UnixSocket &operator=(UnixSocket &&rhs) noexcept {
			FileDescriptor::operator=((FileDescriptor&&)rhs);
			path = rhs.path;
			return *this;
		}
		~UnixSocket() {
			unlink(path);
		}
};

void dummy(int) {}

static void *hookThread(void *args) noexcept {
	signal(SIGUSR1, dummy);

	UnixSocket *serverSock = reinterpret_cast<UnixSocket *>(args);
	if (*serverSock == -1) {
		printf("networkListen %i\n", (int) *serverSock);
		return 0;
	}

	const int syscore = getppid();
	FileDescriptor fd = accept(*serverSock, nullptr, nullptr);
	while (true) {
		if (handleIpc(syscore, fd)) {
			fd = accept(*serverSock, nullptr, nullptr);
			if (fd == -1) {
				// we're done
				return 0;
			}
		}
	}

	return 0;
}

int main() {
	puts("daemon entered");
	AbortServer abortServer{};
	KlogServer klogServer{};
	CommandServer commandServer{};
	pthread_t elfHandler = nullptr;
	UniquePtr<UnixSocket> serverSock{new UnixSocket{"/system_tmp/IPC"}};
	pthread_create(&elfHandler, nullptr, hookThread, serverSock.get());

	abortServer.TcpServer::run();
	klogServer.TcpServer::run();
	commandServer.TcpServer::run();

	// finishes on connect
	abortServer.join();
	puts("abort thread finished");
	commandServer.stop();
	puts("command server done");
	puts("stopping elf handler");
	serverSock = nullptr; // closed the socket
	pthread_kill(elfHandler, SIGUSR1);
	pthread_join(elfHandler, nullptr);
	puts("elf handler done");
	puts("stopping klog server");
	klogServer.stop();
	puts("klog server done");

	// TODO add elf loader with options for process name and type (daemon/game)
	// add whatever other crap people may want
	return 0;
}

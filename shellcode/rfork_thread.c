#define TITLEID_LENGTH 10
#define BREW 0x57455242
#define F_OK 0
#define NULL 0

// AF_UNIX address family
#define AF_UNIX 1

// Maximum length of the sun_path field
#define UNIX_PATH_MAX 104

#define SIN_ZERO_SIZE 8

#define PING 0
#define PONG 1
#define PROCESS_LAUNCHED 1
#define MSG_NOSIGNAL    0x20000         /* do not generate SIGPIPE on EOF */

// NOLINTBEGIN(*)

typedef struct {
	const char *arg0; // /app0/eboot.bin
	const char *path; // /system_ex/app/BREW00000/eboot.bin
	const char *sandboxPath; // /mnt/sandbox/BREW00000_000
	// remaining unknown
} procSpawnArgs;

typedef int (*func_t)(void*);
typedef int (*rfork_thread_t)(int flags, void *stack, func_t func, void *arg);

struct sockaddr_un {
	unsigned char sun_len;
    unsigned char sun_family;    // AF_UNIX
    char sun_path[104];        // Path name
};

typedef struct {
	int sock;
	const func_t inf_loop; // haha open prospero go brrrrrrr
	func_t func;
	int (*socket)(int domain, int type, int protocol);
	int (*close)(int fd);
    int (*connect)(int s, void *name, unsigned int namelen);
	long (*send)(int sockfd, const void *buf, int len, int flags);
	long (*recv)(int sockfd,  void * buf, int len, int flags);
	int (*access)(const char *path, int flags);
} ExtraStuff;

struct result {
	int cmd;
	int pid;
	func_t func;
};

// INFINITE_LOOP: 0xeb 0xfe

// insert a "MOV rfork_thread, R8" at the beginning of the hook shellcode
// insert a "MOV &stuff, R9" at the beginning of the hook shellcode
/*
49 b8 xx xx xx xx xx xx xx xx
49 b9 xx xx xx xx xx xx xx xx
*/

#define SOCK_STREAM 1
#define SERVER_SIZE 15 + 2
#define IPC_PATH_LENGTH 16


static inline int __attribute__((always_inline)) reconnect(const char *restrict path, ExtraStuff *restrict stuff) {
    volatile struct sockaddr_un server;
    stuff->sock = stuff->socket(AF_UNIX, SOCK_STREAM, 0);
	if (stuff->sock == -1) {
		return -1;
	}

	server.sun_len = 0;
    server.sun_family = AF_UNIX;
	__builtin_memcpy((char*)server.sun_path, path, IPC_PATH_LENGTH);

    if (stuff->connect(stuff->sock, (void*)&server, SERVER_SIZE) == -1){
		stuff->close(stuff->sock);
		stuff->sock = -1;
		return -1;
	}
    return 0;
}


static inline int __attribute__((always_inline)) exit_fail(int flags, void *stack, func_t func, procSpawnArgs *restrict arg, rfork_thread_t orig, ExtraStuff *restrict stuff) {
	if (stuff->sock != -1) {
		stuff->close(stuff->sock);
	}
	stuff->sock = -1;
	return orig(flags, stack, func, arg);
}

typedef union {
	const char *str;
	volatile unsigned char *u8;
	volatile unsigned short *u16;
	volatile unsigned int *u32;
	volatile unsigned long *u64;
} string_pointer_t;

#define HOMEBREW_DAEMON_PREFIX_LENGTH 24

static inline int __attribute__((always_inline)) isEqual(const char *restrict src, const char *restrict dst) {
	return __builtin_memcmp(src, dst, HOMEBREW_DAEMON_PREFIX_LENGTH) == 0;
}

static inline int __attribute__((always_inline)) isHomebrewDaemon(const char *path) {
	// /system_ex/app/BREW00000
	volatile unsigned long src[3];
	src[0] = 0x5F6D65747379732F;
	src[1] = 0x422F7070612F7865;
	src[2] = 0x3030303030574552;
	return isEqual((char*)src, path);
}

// /mnt/sandbox/xxxxyyyyy_000/app0/homebrew.elf
#define SANDBOX_PATH_LENGTH 26
#define HOMEBREW_PATH_LENGTH 48

static inline char *restrict __attribute__((always_inline)) copySandboxPath(char *restrict dst, const char *restrict src) {
	return __builtin_memcpy(dst, src, SANDBOX_PATH_LENGTH) + SANDBOX_PATH_LENGTH;
}

static inline int __attribute__((always_inline)) isHomebrew(ExtraStuff *restrict stuff, procSpawnArgs *restrict arg) {
	if (arg == NULL || arg->path == NULL || arg->sandboxPath == NULL) {
		// some safety checks
		return 0;
	}

	if (isHomebrewDaemon(arg->path) == 1) {
		// the homebrew.elf file won't be present for the daemon so we'll explicitly check
		return 1;
	}

	char path[HOMEBREW_PATH_LENGTH];
	volatile unsigned long *dst = (unsigned long *) copySandboxPath(path, arg->sandboxPath);
	dst[0] = 0x6F682F307070612F;
	dst[1] = 0x652E77657262656D;
	dst[2] = 0x666C;
	return stuff->access(path, F_OK) == 0;
}

static int __attribute__((used)) rfork_thread_hook(int flags, void *stack, func_t func, procSpawnArgs *restrict arg, rfork_thread_t orig, ExtraStuff *restrict stuff) {
	if (!isHomebrew(stuff, arg)) {
		return orig(flags, stack, func, arg);
	}

	volatile unsigned long ipc[2];
	ipc[0] = 0x5f6d65747379732f;
	//  tmp/IPC
	ipc[1] = 0x004350492f706d74;  // Little-endian

	if (stuff->sock == -1) {
		if (reconnect((char*)ipc, stuff) == -1) {
			return orig(flags, stack, func, arg);
		}
	} else {
		volatile struct result res;
		res.cmd = PING;
		res.func = 0;
		res.pid = 0;
		if (stuff->send(stuff->sock, (void*)&res, sizeof(res), MSG_NOSIGNAL) == -1) {
			if (reconnect((char*)ipc, stuff) == -1) {
				return exit_fail(flags, stack, func, arg, orig, stuff);
			}
		} else {
			int reply = 0;
			if (stuff->recv(stuff->sock, &reply, sizeof(reply), MSG_NOSIGNAL) == -1) {
				return exit_fail(flags, stack, func, arg, orig, stuff);
			}
			if (reply != PONG) {
				return exit_fail(flags, stack, func, arg, orig, stuff);
			}
		}
	}

	const int pid = orig(flags, stack, stuff->inf_loop, arg);
	struct result res = {
		.cmd = PROCESS_LAUNCHED,
		.pid = pid,
		.func = func
	};

	// we must always write a response so the daemon doesn't get stuck
	stuff->send(stuff->sock, (void *)&res, sizeof(res),  MSG_NOSIGNAL);
	if (pid == -1) {
		return exit_fail(flags, stack, func, arg, orig, stuff);
	}

	return pid;
}

// NOLINTEND(*)

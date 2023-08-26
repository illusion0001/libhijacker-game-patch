#define TITLEID_LENGTH 10
#define BREW 0x57455242

// AF_UNIX address family
#define AF_UNIX 1

#define AF_INET 2

// Maximum length of the sun_path field
#define UNIX_PATH_MAX 104

typedef unsigned int sa_family_t;

#define	SOL_SOCKET 0xffff
#define	SO_SNDTIMEO 0x1005
#define SIN_ZERO_SIZE 8

#define TIMEOUT 200
#define PORT 0x0f27 // 9999
#define LOCALHOST 0x0100007f // 127.0.0.1

#define PING 0
#define PONG 1
#define PROCESS_LAUNCHED 1
#define IPC_SOC 0x4350492f706d745f6d65747379732f

// NOLINTBEGIN(*)

typedef struct {
	const char *arg0; // /app0/eboot.bin
	const char *path; // /system_ex/app/BREW00000/eboot.bin
	const char *sandboxPath; // /mnt/sandbox/BREW00000_000
	// remaining unknown
} procSpawnArgs;

typedef int (*func_t)(void*);
typedef int (*rfork_thread_t)(int flags, void *stack, func_t func, void *arg);

struct sockaddr_in {
	unsigned char sin_len;
	unsigned char sin_family;
	unsigned short sin_port;
	unsigned int sin_addr;
	//char sin_zero[SIN_ZERO_SIZE];
	unsigned long sin_zero;
};

struct sockaddr_un {
	unsigned char sun_len;
    unsigned char sun_family;    // AF_UNIX
    char sun_path[104];        // Path name
};

struct timeval {
	long long tv_sec;
	long long tv_usec;
};

typedef struct {
	int sock;
	const func_t inf_loop; // haha open prospero go brrrrrrr
	func_t func;
	int (*socket)(int domain, int type, int protocol);
	int (*close)(int fd);
	int (*connect)(int s, void *name, unsigned int namelen);
	long (*_read)(int fd, void * buf, unsigned long nbytes);
	long (*_write)(int fd, const void *buf, unsigned long nbytes);
	int (*setsockopt)(int s, int level, int optname, const void *optval, unsigned int optlen);
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

/*
static inline int __attribute__((always_inline)) reconnect(ExtraStuff *restrict stuff) {
	// establish connection
	stuff->sock = stuff->socket(AF_INET, SOCK_STREAM, 0);
	if (stuff->sock == -1) {
		return -1;
	}

	volatile struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = TIMEOUT;

	if (stuff->setsockopt(stuff->sock, SOL_SOCKET, SO_SNDTIMEO, (void*)&timeout, sizeof(struct timeval)) == -1) {
		return -1;
	}

	volatile struct sockaddr_in addr;
	addr.sin_len = sizeof(struct sockaddr_in);
	addr.sin_family = AF_INET;
	addr.sin_port = PORT;
	addr.sin_addr = LOCALHOST;
	addr.sin_zero = 0;
	if (stuff->connect(stuff->sock, (struct sockaddr_in *)&addr, sizeof(addr)) == -1) {
		return -1;
	}
	return 0;
}
*/


static inline int __attribute__((always_inline)) reconnect(ExtraStuff *restrict stuff) {
    volatile struct sockaddr_un server;
    stuff->sock = stuff->socket(AF_UNIX, SOCK_STREAM, 0);
	if (stuff->sock == -1) {
		return -1;
	}

	server.sun_len = 0;
    server.sun_family = AF_UNIX;
    // /system_tmp/IPC backwards
	volatile unsigned long long *str = (unsigned long long*)&server.sun_path[0];
	// /system_
	str[0] = 0x5f6d65747379732f;
	//  tmp/IPC
	str[1] = 0x004350492f706d74;  // Little-endian

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

static inline int __attribute__((always_inline)) isHomebrew(const char *path) {
	// /system_ex/app/BREW
	const unsigned long long *ptr = (unsigned long long *)path;
	if (ptr == 0) {
		return 0;
	}
	if (*ptr++ != 0x5F6D65747379732F) { // "/system_"
		return 0;
	}
	if (*ptr++ != 0x422F7070612F7865) { // "ex/app/B"
		return 0;
	}
	// the buffer is padded out with enough zeroes
	if ((*ptr & 0x0000000000ffffff) != 0x0000000000574552) { // REW\0\0\0\0\0
		return 0;
	}
	return 1;
}

static int __attribute__((used)) rfork_thread_hook(int flags, void *stack, func_t func, procSpawnArgs *restrict arg, rfork_thread_t orig, ExtraStuff *restrict stuff) {
	if (!isHomebrew(arg->path)) {
		return orig(flags, stack, func, arg);
	}
	if (stuff->sock == -1) {
		if (reconnect(stuff) == -1) {
			return orig(flags, stack, func, arg);
		}
	} else {
		volatile struct result res;
		res.cmd = PING;
		res.func = 0;
		res.pid = 0;
		if (stuff->_write(stuff->sock, (void*)&res, sizeof(res)) == -1) {
			if (reconnect(stuff) == -1) {
				return exit_fail(flags, stack, func, arg, orig, stuff);
			}
		} else {
			int reply = 0;
			if (stuff->_read(stuff->sock, &reply, sizeof(reply)) == -1) {
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
	stuff->_write(stuff->sock, (void *)&res, sizeof(res));
	if (pid == -1) {
		return exit_fail(flags, stack, func, arg, orig, stuff);
	}

	return pid;
}

// NOLINTEND(*)

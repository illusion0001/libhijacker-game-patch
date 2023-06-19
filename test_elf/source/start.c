// Required header
#include <stdint.h>
#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ps5/payload_main.h>

/*
struct payload_args
{
	dlsym_t* dlsym;             // 0x00
	int *rwpipe;                // 0x08
	int *rwpair;                // 0x10
	uint64_t kpipe_addr;        // 0x18
	uint64_t kdata_base_addr;   // 0x20
	int *payloadout;            // 0x28
};
*/

// Store necessary sockets/pipe for corruption.
int _rw_pipe[2];
int _master_sock;
int _victim_sock;
uint64_t _pipe_addr;
uintptr_t kernel_base;

extern int puts(const char*);
extern int printf(const char*, ...);

extern size_t _write(int fd, const void *buf, size_t nbyte);
extern size_t _read(int fd, void *buf, size_t nbyte);

// Arguments passed by way of entrypoint arguments.
void kernel_init_rw(struct payload_args *__restrict args) {
	_rw_pipe[0]  = args->rwpipe[0];
	_rw_pipe[1]  = args->rwpipe[1];
	_master_sock = args->rwpair[0];
	_victim_sock = args->rwpair[1];
	_pipe_addr   = args->kpipe_addr;
	kernel_base  = args->kdata_base_addr;
}

// Internal kwrite function - not friendly, only for setting up better primitives.
void kwrite(uint64_t addr, uint64_t *data) {
	setsockopt(_master_sock, IPPROTO_IPV6, IPV6_PKTINFO, (uint64_t[3]){addr, 0, 0}, 0x14);
	setsockopt(_victim_sock, IPPROTO_IPV6, IPV6_PKTINFO, data, 0x14);
}

// Public API function to write kernel data.
void kernel_copyin(const void *src, uint64_t kdest, size_t length) {
	// Set pipe flags
	kwrite(_pipe_addr, (uint64_t[3]){0, 0x4000000000000000, 0});

	// Set pipe data addr
	kwrite(_pipe_addr + 0x10, (uint64_t[3]){kdest, 0, 0});

	// Perform write across pipe
	_write(_rw_pipe[1], src, length);
}

// Public API function to read kernel data.
void kernel_copyout(uint64_t ksrc, void *dest, size_t length) {
	// Set pipe flags
	kwrite(_pipe_addr, (uint64_t[3]){0x4000000040000000, 0x4000000000000000, 0});

	// Set pipe data addr
	kwrite(_pipe_addr + 0x10, (uint64_t[3]){ksrc, 0, 0});

	// Perform read across pipe
	_read(_rw_pipe[0], dest, length);
}

extern int main(int argc, const char **argv);

void __attribute__((noreturn)) _start(struct payload_args *__restrict args) {
	kernel_init_rw(args);
	exit(main(0, NULL));
}

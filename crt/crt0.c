#include <fcntl.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ps5/payload_main.h>

#define STDOUT 1
#define STDERR 2
#define SYSCALL_OFFSET 7
#define LIBKERNEL_HANDLE 0x2001

// NOLINTBEGIN(*) c sucks so screw linting it

// Store necessary sockets/pipe for corruption.
int _rw_pipe[2];
int _master_sock;
int _victim_sock;
uint64_t _pipe_addr;
uintptr_t kernel_base;
uintptr_t syscall_addr = 0;

extern int puts(const char*);
extern int printf(const char*, ...);

extern size_t _write(int fd, const void *buf, size_t nbyte);
extern size_t _read(int fd, void *buf, size_t nbyte);

// Arguments passed by way of entrypoint arguments.
static void kernel_init_rw(struct payload_args *__restrict args) {
	_rw_pipe[0]  = args->rwpipe[0];
	_rw_pipe[1]  = args->rwpipe[1];
	_master_sock = args->rwpair[0];
	_victim_sock = args->rwpair[1];
	_pipe_addr   = args->kpipe_addr;
	kernel_base  = args->kdata_base_addr;
}

// Internal kwrite function - not friendly, only for setting up better primitives.
static void kwrite(uint64_t addr, uint64_t *data) {
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

extern void (*__preinit_array_start[])(void) __attribute__((weak));
extern void (*__preinit_array_end[])(void) __attribute__((weak));
extern void (*__init_array_start[])(void) __attribute__((weak));
extern void (*__init_array_end[])(void) __attribute__((weak));
extern void (*__fini_array_start[])(void) __attribute__((weak));
extern void (*__fini_array_end[])(void) __attribute__((weak));
extern uint8_t __bss_start __attribute__((weak));
extern uint8_t __bss_end __attribute__((weak));

static void _preinit(void) {
	const size_t length = __preinit_array_end - __preinit_array_start;
	for (size_t i = 0; i < length; i++) {
		__preinit_array_start[i]();
	}
}

static void _init(void) {
	const size_t length = __init_array_end - __init_array_start;
	for (size_t i = 0; i < length; i++) {
		__init_array_start[i]();
	}
}

static void _fini(void) {
	const size_t length = __fini_array_end - __fini_array_start;
	for (size_t i = 0; i < length; i++) {
		__fini_array_start[i]();
	}
}

void __attribute__((noreturn)) _start(struct payload_args *__restrict args) {

	// zero .bss
	__builtin_memset(&__bss_start, 0, &__bss_end - &__bss_start);

	// init stdout, stderr and kernelrw first
	int fd = open("/dev/console", O_WRONLY);
	dup2(fd, STDOUT);
	dup2(STDOUT, STDERR);
	kernel_init_rw(args);

	// preinit and then init
	_preinit();
	_init();

	// register _fini
	atexit(_fini);

	uintptr_t get_authinfo = 0;
	args->dlsym(LIBKERNEL_HANDLE, "get_authinfo", &get_authinfo);
	syscall_addr = get_authinfo + SYSCALL_OFFSET;

	// run main
	exit(main(0, NULL));
}

int __attribute__ ((naked, weak, noinline)) mdbg_call() {
	__asm__ volatile(
		"mov $573, %rax\n"
		"jmp *syscall_addr(%rip)\n"
	);
}

int __attribute__ ((naked, weak, noinline)) ptrace() {
	__asm__ volatile(
		"mov $26, %rax\n"
		"jmp *syscall_addr(%rip)\n"
	);
}

int __attribute__ ((naked, weak, noinline)) nmount() {
	__asm__ volatile(
		"mov $378, %rax\n"
		"jmp *syscall_addr(%rip)\n"
	);
}

// NOLINTEND(*)

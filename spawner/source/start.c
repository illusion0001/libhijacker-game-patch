// Required header

#include "main.h"
#include <ps5/payload_main.h>
#include <ps5/kernel.h>
#include <stddef.h>

#define libkernel 0x2001
#define nullptr 0

extern int main(int argc, const char **argv);

static __attribute__ ((used)) void *f_sceKernelDlsym = nullptr;
int __attribute__ ((naked)) sceKernelDlsym(int lib, const char *name, void **fun) {
	__asm__ volatile("jmp *f_sceKernelDlsym(%rip)");
}

static __attribute__ ((used)) void *f_sceKernelLoadStartModule = nullptr;
int __attribute__ ((naked)) sceKernelLoadStartModule(const char *name, size_t argc, const void *argv, uint32_t flags, void *unknown, int *result) {
	__asm__ volatile("jmp *f_sceKernelLoadStartModule(%rip)");
}

#define STUB(fname) \
static __attribute__ ((used)) void *f_##fname = nullptr;\
int __attribute__ ((naked)) fname() {\
	__asm__ volatile("jmp *f_"#fname"(%rip)");\
}

// int sceKernelLoadStartModule(char *name, size_t argc, void *argv, uint32_t flags, void *unknown, int *result)

static __attribute__ ((used)) void *f_usleep = nullptr;
int __attribute__ ((naked)) usleep(unsigned int useconds) {
	__asm__ volatile("jmp *f_usleep(%rip)");
}

static __attribute__ ((used)) void *f_close = nullptr;
int __attribute__ ((naked)) close(int fd) {
	__asm__ volatile("jmp *f_close(%rip)");
}

static __attribute__ ((used)) void *f_puts = nullptr;\
int __attribute__ ((naked)) puts(const char *msg) {\
	__asm__ volatile("jmp *f_puts(%rip)");\
}

static __attribute__ ((used)) void *f_free = nullptr;\
void __attribute__ ((naked)) free(void *msg) {\
	__asm__ volatile("jmp *f_free(%rip)");\
}
static __attribute__ ((used)) void *f_malloc = nullptr;\
void *__attribute__ ((naked)) malloc(size_t len) {\
	__asm__ volatile("jmp *f_malloc(%rip)");\
}



STUB(getpid)
STUB(memset)
STUB(putchar)
STUB(memcpy)
STUB(memcmp)
STUB(socket)
STUB(bind)
STUB(listen)
STUB(accept)
STUB(setsockopt)
STUB(_write)
STUB(_read)
STUB(printf)
STUB(_ZdaPv)
STUB(_Znam)
STUB(_Znwm)
STUB(_ZdlPv)
STUB(strstr)
STUB(strlen)
STUB(strnlen)
STUB(sysctlbyname)
STUB(strncpy)
STUB(strncmp)
STUB(__error)
STUB(strerror)
STUB(sceKernelPrintBacktraceWithModuleInfo)

#define LINK(lib, fname) sceKernelDlsym(lib, #fname, &f_##fname)
#define LIBKERNEL_LINK(fname) LINK(libkernel, fname)
#define LIBC_LINK(fname) LINK(libc, fname)

void _start(struct payload_args *args) {

	f_sceKernelDlsym = args->dlsym;
	LIBKERNEL_LINK(sceKernelLoadStartModule);
	int libc = sceKernelLoadStartModule("libSceLibcInternal.sprx", 0, 0, 0, 0, 0);
	LIBC_LINK(puts);
	puts("_start entered");
	LIBKERNEL_LINK(usleep);
	LIBKERNEL_LINK(getpid);
	LIBKERNEL_LINK(socket);
	LIBKERNEL_LINK(bind);
	LIBKERNEL_LINK(listen);
	LIBKERNEL_LINK(accept);
	LIBKERNEL_LINK(setsockopt);
	LIBKERNEL_LINK(_write);
	LIBKERNEL_LINK(_read);
	LIBKERNEL_LINK(close);
	LIBKERNEL_LINK(sysctlbyname);
	LIBKERNEL_LINK(__error);
	LIBKERNEL_LINK(sceKernelPrintBacktraceWithModuleInfo);



	LIBC_LINK(_Znwm);
	LIBC_LINK(_Znam);
	LIBC_LINK(_ZdlPv);
	LIBC_LINK(_ZdaPv);
	LIBC_LINK(memset);
	LIBC_LINK(putchar);
	LIBC_LINK(malloc);
	LIBC_LINK(free);
	LIBC_LINK(memcpy);
	LIBC_LINK(memcmp);
	LIBC_LINK(printf);

	LIBC_LINK(strstr);
	LIBC_LINK(strlen);
	LIBC_LINK(strnlen);
	LIBC_LINK(strncpy);
	LIBC_LINK(strncmp);
	LIBC_LINK(strerror);

	kernel_base = args->kdata_base_addr;
	kernel_init_rw(args->rwpair[0], args->rwpair[1], args->rwpipe, args->kpipe_addr);
	*args->payloadout = main(0, NULL);
}

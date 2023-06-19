#pragma  once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

void kernel_init_rw(int master_sock, int victim_sock, int *rw_pipe, uint64_t pipe_addr);
void kernel_copyin(void *src, uint64_t kdest, size_t length);
void kernel_copyout(uint64_t ksrc, void *dest, size_t length);

#ifdef __cplusplus
}
#endif

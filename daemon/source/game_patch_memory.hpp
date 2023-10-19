#pragma once

extern "C"
{
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
    void *malloc(size_t size);
    void free(void *ptr);
}

enum write_flag : uint32_t
{
    no_flag = 0,
    isOffsetConfigureOutput = 1 << 1,
    isOffsetVideoModeSupported = 1 << 2,
};

void dump_bytes_vm(pid_t pid, uint64_t addr, size_t bytes_size);

void write_bytes(pid_t pid, uint64_t addr, const char *hexString, enum write_flag special_flag = no_flag);
void write_bytes32(pid_t pid, uint64_t addr, const uint32_t val);
void write_bytes64(pid_t pid, uint64_t addr, const size_t val);
void write_float32(pid_t pid, uint64_t addr, const float val);
void write_float64(pid_t pid, uint64_t addr, const double val);
void write_string(pid_t pid, uint64_t addr, const char *string);
void write_wstring(pid_t pid, uint64_t addr, const wchar_t *string);
uint8_t *PatternScan(const uint64_t module_base, const uint64_t module_size, const char *signature);

bool patchShellCore(const pid_t app_pid, const uint64_t shellcore_base, const uint64_t shellcore_size);
bool UnPatchShellCore(const pid_t app_pid);

#define NO_ASLR_BASE 0x00400000
#define NO_ASLR(addr) (text_base + (addr - NO_ASLR_BASE))
#define BASE_ASLR(addr) (text_base + addr)
#define BASE_ASLR_OFFSET(base_addr, addr) (text_base + (addr - base_addr))

#define startsWith(str1, str2) (strncmp(str1, str2, __builtin_strlen(str2)) == 0)

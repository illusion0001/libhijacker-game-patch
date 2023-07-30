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

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

void dump_bytes_vm(pid_t pid, u64 addr, size_t bytes_size);

void write_bytes(pid_t pid, u64 addr, const char *hexString);
void write_bytes32(pid_t pid, u64 addr, const u32 val);
void write_bytes64(pid_t pid, u64 addr, const s64 val);
void write_string(pid_t pid, u64 addr, const char *string);
void write_wstring(pid_t pid, u64 addr, const wchar_t *string);

#define NO_ASLR_BASE 0x00400000
#define NO_ASLR(addr) (text_base + (addr - NO_ASLR_BASE))
#define BASE_ASLR(addr) (text_base + addr)

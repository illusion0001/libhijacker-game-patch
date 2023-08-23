#pragma once

extern "C" {
	#include <stdio.h>
	size_t print_null(...);
}

#ifdef DEBUG
#define _puts(x) printf("%s:%d: %s\n", __PRETTY_FUNCTION__, __LINE__, x)
#define _printf(...) { \
	printf("%s:%d ", __PRETTY_FUNCTION__, __LINE__); \
	printf(__VA_ARGS__); \
}
#define print_ret(func) printf("%s:%d: " #func ": 0x%08x\n", __PRETTY_FUNCTION__, __LINE__, func)
#else
#define _puts(x) print_null("%s:%d: %s\n", __PRETTY_FUNCTION__, __LINE__, x)
#define _printf(...) { \
	print_null("%s:%d ", __PRETTY_FUNCTION__, __LINE__); \
	print_null(__VA_ARGS__); \
}
#define print_ret(func) print_null("%s:%d: " #func ": 0x%08x\n", __PRETTY_FUNCTION__, __LINE__, func)
#endif

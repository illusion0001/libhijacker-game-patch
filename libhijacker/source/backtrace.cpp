#include "backtrace.hpp"

const Frame * __attribute__((naked)) getFramePointer() {
	__asm__ volatile(
		"push %rbp\n"
		"pop %rax\n"
		"ret\n"
	);
}

extern "C" int puts(const char*);

static uintptr_t __attribute__((naked, noinline)) getTextStart() {
	asm volatile(
		"lea __text_start(%rip), %rax\n"
		"ret\n"
	);
}

static uintptr_t __attribute__((naked, noinline)) getTextEnd() {
	asm volatile(
		"lea __text_stop(%rip), %rax\n"
		"ret\n"
	);
}

void printBacktrace() {
	const uintptr_t start = getTextStart();
	const uintptr_t stop = getTextEnd();
	__builtin_printf(".text: 0x%08llx\n", (unsigned long long)start);
	puts("---backtrace start---");
	for (const Frame *__restrict frame = getFramePointer(); frame != nullptr; frame = frame->next) {
		if (frame->addr != 0) [[likely]] {
			if (frame->addr >= start && frame->addr <= stop) {
				__builtin_printf("0x%llx\n", (unsigned long long)frame->addr - start);
			}
		}
	}
	puts("---backtrace end---");
}

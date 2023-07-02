
#include <stdio.h>

static void __attribute__((constructor)) test_constructor() {
	puts("hello world");
}

static void __attribute__((destructor)) test_destructor() {
	puts("goodbye world");
}

int main() {
	puts("main entered");
	return 0;
}

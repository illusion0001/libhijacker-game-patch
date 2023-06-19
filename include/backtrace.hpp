#pragma once

extern "C" {
	#include <stdint.h>
}

struct Frame {
	Frame *next;
	uintptr_t addr;
};

const Frame *getFramePointer();


extern "C" {
	#include <stdint.h>
	#include <sys/types.h>
	#include <sys/sysctl.h>
}

static uint32_t version;
static size_t allprocOffset;

static uint32_t getSystemSwVersion() {
	if (version != 0) [[likely]] {
		return version;
	}
	size_t size = 4;
	sysctlbyname("kern.sdk_version", &version, &size, nullptr, 0);
	return version;
}

namespace offsets {

size_t allproc() {
	if (allprocOffset != 0) [[likely]] {
		return allprocOffset;
	}
	switch(getSystemSwVersion() & 0xffff0000) {
		case 0x3000000:
			allprocOffset = 0x276DC58;
			break;
		case 0x3100000:
			allprocOffset = 0x276DC58;
			break;
		case 0x3200000:
			allprocOffset = 0x276DC58;
			break;
		case 0x3210000:
			allprocOffset = 0x276DC58;
			break;
		case 0x4020000:
			allprocOffset = 0x27EDCB8;
			break;
		case 0x4030000:
			allprocOffset = 0x27EDCB8;
			break;
		case 0x4500000:
			allprocOffset = 0x27EDCB8;
			break;
		case 0x4510000:
			allprocOffset = 0x27EDCB8;
			break;
		default:
			allprocOffset = -1;
			break;
	}
	return allprocOffset;
}

size_t security_flags() {
	switch(getSystemSwVersion() & 0xffff0000) {
		case 0x3000000:
			return 0x6466474;
		case 0x3100000:
			return 0x6466474;
		case 0x3200000:
			return 0x6466474;
		case 0x3210000:
			return 0x6466474;
		case 0x4020000:
			return 0x6505474;
		case 0x4030000:
			return 0x6506474;
		case 0x4500000:
			return 0x6506474;
		case 0x4510000:
			return 0x6506474;
		default:
			return -1;
	}
}

size_t qa_flags() {
	switch(getSystemSwVersion() & 0xffff0000) {
		case 0x3000000:
			return 0x6466498;
		case 0x3100000:
			return 0x6466498;
		case 0x3200000:
			return 0x6466498;
		case 0x3210000:
			return 0x6466498;
		case 0x4020000:
			return 0x6505498;
		case 0x4030000:
			return 0x6506498;
		case 0x4500000:
			return 0x6506498;
		case 0x4510000:
			return 0x6506498;
		default:
			return -1;
	}
}

size_t utoken_flags() {
	switch(getSystemSwVersion() & 0xffff0000) {
		case 0x3000000:
			return 0x6466500;
		case 0x3100000:
			return 0x6466500;
		case 0x3200000:
			return 0x6466500;
		case 0x3210000:
			return 0x6466500;
		case 0x4020000:
			return 0x6505500;
		case 0x4030000:
			return 0x6506500;
		case 0x4500000:
			return 0x6506500;
		case 0x4510000:
			return 0x6506500;
		default:
			return -1;
	}
}

size_t root_vnode() {
	switch(getSystemSwVersion() & 0xffff0000) {
		case 0x3000000:
			return 0x67AB4C0;
		case 0x3100000:
			return 0x67AB4C0;
		case 0x3200000:
			return 0x67AB4C0;
		case 0x3210000:
			return 0x67AB4C0;
		case 0x4020000:
			return 0x66E64C0;
		case 0x4030000:
			return 0x66E74C0;
		case 0x4500000:
			return 0x66E74C0;
		case 0x4510000:
			return 0x66E74C0;
		default:
			return -1;
	}
}

} // offsets

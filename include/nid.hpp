#pragma once

extern "C" {
	#include <stdint.h>
	#include <stddef.h>
}

namespace {

static constexpr size_t NID_LENGTH = 11;

}

union Nid {

	char str[NID_LENGTH + 1]; // 12th character is for NULL terminator to allow constexpr constructor
	struct __attribute__((packed)) data_t {
		int_fast64_t low;
		int_fast32_t hi;
	} data;

	Nid() noexcept = default;

	explicit constexpr Nid(const char *nid) noexcept : str{} {
		__builtin_memcpy(str, nid, NID_LENGTH);
		str[NID_LENGTH] = '\0';
	}

	constexpr int_fast64_t operator<=>(const Nid& rhs) const {
		auto i = data.low - rhs.data.low;
		if (i == 0) [[unlikely]] {
			return data.hi - rhs.data.hi;
		}
    	return i;
	}

	constexpr bool operator==(const Nid &rhs) const {
		return data.low == rhs.data.low && data.hi == rhs.data.hi;
	}

	constexpr void setNullTerminator() {
		str[NID_LENGTH] = '\0';
	}
};

namespace nid {

static inline constexpr Nid get_authinfo{"igMefp4SAv0"};
static inline constexpr Nid sceSystemServiceGetAppStatus{"t5ShV0jWEFE"};
static inline constexpr Nid sceSystemServiceAddLocalProcess{"0cl8SuwosPQ"};
static inline constexpr Nid socketpair{"MZb0GKT3mo8"};
static inline constexpr Nid usleep{"QcteRwbsnV0"};
static inline constexpr Nid _errno{"9BcDykPmo1I"};
static inline constexpr Nid sceSysmoduleLoadModuleInternal{"39iV5E1HoCk"};
static inline constexpr Nid sceSysmoduleLoadModuleByNameInternal{"CU8m+Qs+HN4"};
static inline constexpr Nid mmap{"BPE9s9vQQXo"};
static inline constexpr Nid munmap{"UqDGjXA5yUM"};
static inline constexpr Nid sceKernelJitCreateSharedMemory{"avvJ3J0H0EY"};
static inline constexpr Nid socket{"TU-d9PfIHPM"};
static inline constexpr Nid pipe{"-Jp7F+pXxNg"};
static inline constexpr Nid sceKernelDlsym{"LwG8g3niqwA"};
static inline constexpr Nid setsockopt{"fFxGkxF2bVo"};
static inline constexpr Nid execve{"kdguLiAheLI"};
static inline constexpr Nid _nanosleep{"NhpspxdjEKU"};

}

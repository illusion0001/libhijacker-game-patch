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

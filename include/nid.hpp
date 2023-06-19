#pragma once

extern "C" {
	#include <stdint.h>
}

union Nid {

	char str[12]; // 12th character is for NULL terminator to allow constexpr constructor
	struct __attribute__((packed)) data_t {
		int_fast64_t low;
		int_fast32_t hi;
	} data;

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
};

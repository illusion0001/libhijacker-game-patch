#pragma once

#include "b64.hpp"
#include "sha1.hpp"
#include "util.hpp"

static inline constexpr void fillNid(char (&buf)[NID_LENGTH+1], const StringView &sym) {
    uint8_t encodedDigest[9]{};
	uint8_t sha1[20]{};
	genSha1(sha1, sym);
	for (int i = 0; i < 8; i++) {
		encodedDigest[i] = sha1[8 - 1 - i];
	}
	b64encode(buf, encodedDigest);
}

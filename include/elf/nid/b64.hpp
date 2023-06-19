#pragma once

extern "C" {
	#include <stdint.h>
}


static inline constexpr char encoder[]{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-"};
static constexpr int NID_LENGTH = 11;

static inline constexpr void b64encode(char (&dest)[NID_LENGTH+1], const unsigned char *src) {
	for (int i = 0, j = 0; j < NID_LENGTH;) {
		uint32_t a = src[i++];
		uint32_t b = src[i++];
		uint32_t c = src[i++];

		uint32_t abc = (a << 16) | (b << 8) | c;

		dest[j++] = encoder[(abc >> 18) & 0x3F];
		dest[j++] = encoder[(abc >> 12) & 0x3F];
		dest[j++] = encoder[(abc >> 6) & 0x3F];
		dest[j++] = encoder[abc & 0x3F];
	}
	dest[NID_LENGTH] = 0;
}



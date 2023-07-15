#pragma once

#include "hijacker/hijacker.hpp"
#include "hijacker/spawner.hpp"

class ScopedSuspender {
	Hijacker *hijacker;

	public:
		ScopedSuspender(Hijacker *hijacker) : hijacker(hijacker) { hijacker->suspend(); }
		ScopedSuspender(const ScopedSuspender&) = delete;
		ScopedSuspender(ScopedSuspender&&) = delete;
		ScopedSuspender &operator=(const ScopedSuspender&) = delete;
		ScopedSuspender &operator=(ScopedSuspender&&) = delete;
		~ScopedSuspender() { hijacker->resume(); }
};

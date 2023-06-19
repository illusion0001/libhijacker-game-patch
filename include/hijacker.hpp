#pragma once

#include "hijacker/hijacker.hpp"
#include "hijacker/spawner.hpp"

class ScopedSuspender {
	Hijacker *hijacker;

	public:
		ScopedSuspender(Hijacker *hijacker) : hijacker(hijacker) { hijacker->suspend(); }
		~ScopedSuspender() { hijacker->resume(); }
};

#pragma once
#include "kernel.hpp"

// "allocates" memory from another process out of EXISTING process memory
class ProcessMemoryAllocator {
	const SharedLibSection *section;
	size_t allocated;

	public:
		ProcessMemoryAllocator(decltype(nullptr)) : section(nullptr), allocated() {}
		ProcessMemoryAllocator(const SharedLibSection *section) : section(section), allocated() {}
		ProcessMemoryAllocator(const ProcessMemoryAllocator &rhs) = default;
		ProcessMemoryAllocator &operator=(const ProcessMemoryAllocator &rhs) = default;

		/**
		 * "Releases" all the allocated memory
		 */
		void release() {
			allocated = 0;
		}

		/**
		 * Allocated virtual memory from the end of the existing section.
		 * It is allocated from the end because this portion of memory is likely unused.
		 * @param size the size of memory to "allocate"
		 * @return the virtual address for the requested memory
		 */
		uintptr_t allocate(size_t size) {
			if ((size & 0xf) != 0) {
				size = (size & ~0xf) + 0x10;
			}
			allocated += size;
			return section->end() - allocated;
		}

		operator bool() const {
			return section != nullptr;
		}

		bool operator==(decltype(nullptr)) const {
			return section == nullptr;
		}
};

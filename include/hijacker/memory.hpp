#pragma once

extern "C" {
	#include <stdint.h>
}

#include "dbg.hpp"

template <typename T>
class ProcessPointer;

template <typename T>
class ProcessReference {
	uintptr_t addr;
	int pid;

	friend class ProcessReference<T>;

	public:
		ProcessReference() = default;
		ProcessReference(int pid, uintptr_t addr) : addr(addr), pid(pid) {}
		ProcessReference(const ProcessReference &rhs) : addr(rhs.addr), pid(rhs.pid) {}
		ProcessReference(ProcessReference&&) = delete;
		ProcessReference &operator=(const ProcessReference &rhs) {
			addr = rhs.addr;
			pid = rhs.pid;
			return *this;
		}
		ProcessReference &operator=(ProcessReference&&) = delete;
		ProcessReference &operator=(T value) {
			dbg::write(pid, addr, &value, sizeof(value));
			return *this;
		}
		operator T() const {
			T value;
			dbg::read(pid, addr, &value, sizeof(T));
			return value;
		}
};

template <>
class ProcessReference<bool> {
	friend class ProcessReference<bool>;

	uintptr_t addr;
	int pid;

	public:
		ProcessReference(int pid, uintptr_t addr) : addr(addr), pid(pid) {}
		ProcessReference(const ProcessReference &rhs) : addr(rhs.addr), pid(rhs.pid) {}
		ProcessReference(ProcessReference&&) = delete;
		ProcessReference &operator=(const ProcessReference &rhs) {
			addr = rhs.addr;
			pid = rhs.pid;
			return *this;
		}
		ProcessReference &operator=(bool value) {
			dbg::write(pid, addr, &value, sizeof(value));
			return *this;
		}
		explicit operator bool() const {
			bool value;
			dbg::read(pid, addr, &value, sizeof(bool));
			return value;
		}
};

template <typename T>
class ProcessPointer {
	ProcessReference<T> ref;

	public:
		ProcessPointer() : ref{} {}
		ProcessPointer(int pid, uintptr_t addr) : ref(pid, addr) {}
		ProcessPointer(const ProcessPointer&) = default;
		ProcessPointer(ProcessPointer&&) = default;
		ProcessPointer &operator=(const ProcessPointer&) = default;
		ProcessPointer &operator=(ProcessPointer&&) = default;
		const ProcessReference<T> &operator*() const { return ref; }
		ProcessReference<T> &operator*() { return ref; }
		T get() const {
			T value{};
			dbg::read(ref.pid, ref.addr, &value, sizeof(T));
			return value;
		}
		uintptr_t address() const { return ref.addr; }
};

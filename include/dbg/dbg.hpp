#pragma once

#include "args.hpp"
#include "util.hpp"

extern "C" {
	int pthread_getthreadid_np();
	int getpid();
	size_t strnlen(const char *s, size_t maxlen);
}

namespace dbg {

class IdArray {
	int *ptr;
	size_t size;

public:
	IdArray(decltype(nullptr)) : ptr(nullptr), size() {}
	IdArray(const int *ptr, size_t length) : ptr(new int[length]), size(length) {
		__builtin_memcpy(this->ptr, ptr, length * sizeof(int));
	}
	IdArray(const IdArray&) = delete;
	IdArray& operator=(const IdArray&) = delete;
	IdArray(IdArray &&rhs) : ptr(rhs.ptr), size(rhs.size) { rhs.ptr = nullptr; }
	IdArray& operator=(IdArray &&rhs) {
		delete[] ptr;
		ptr = rhs.ptr;
		rhs.ptr = nullptr;
		size = rhs.size;
		return *this;
	}
	~IdArray() {
		delete[] ptr;
	}

	int operator[](unsigned long i) const {
		if (ptr == nullptr) {
			return -1;
		}
		return ptr[i];
	}

	explicit operator bool() const { return ptr != nullptr; }

	size_t length() const { return size; }

	const int *begin() const { return ptr; }
	const int *end() const { return ptr + size; }
	bool contains(int value) const {
		for (int id : *this) {
			if (id == value) {
				return true;
			}
		}
		return false;
	}
};

class ProcessInfo {
	static constexpr unsigned int BUF_LENGTH = 0x51c;

	UniquePtr<uint8_t[]> buf;
	StringView _name;
	StringView _path;
	int _pid;

	void fillInfo();

public:

	ProcessInfo(int pid) : buf(new uint8_t[BUF_LENGTH]), _name(nullptr), _path(nullptr), _pid(pid) {
		fillInfo();
		size_t nameLength = strnlen((char *)buf.get() + 0x18, 0x20);
		_name = {(char *)buf.get() + 0x18, nameLength};
		size_t pathLength = strnlen((char *)buf.get() + 0x38, 0x400);
		_path = {(char *)buf.get() + 0x38, pathLength};
	}
	ProcessInfo(const ProcessInfo&) = delete;
	ProcessInfo &operator=(const ProcessInfo&) = delete;
	ProcessInfo(ProcessInfo &&rhs) : buf(rhs.buf.release()), _name(rhs._name), _path(rhs._path), _pid(rhs._pid) {}
	ProcessInfo &operator=(ProcessInfo &&rhs) {
		buf = rhs.buf.release();
		_name = rhs._name;
		_path = rhs._path;
		_pid = rhs._pid;
		return *this;
	}

	const StringView &name() const {
		return _name;
	}

	const StringView &path() const {
		return _path;
	}

	int pid() const {
		return _pid;
	}
};


class ThreadInfo {
	static constexpr unsigned int BUF_LENGTH = 0x15c;

	UniquePtr<uint8_t[]> buf;
	StringView _name;
	int _pid;
	int _tid;

	void fillInfo();

public:

	ThreadInfo() : buf(new uint8_t[BUF_LENGTH]), _name(nullptr), _pid(getpid()), _tid(pthread_getthreadid_np()) {
		fillInfo();
		size_t nameLength = strnlen((char *)buf.get() + 0x64, 0x20);
		_name = {(char *)buf.get() + 0x64, nameLength};
	}

	ThreadInfo(int tid) : buf(new uint8_t[BUF_LENGTH]), _name(nullptr), _pid(getpid()), _tid(tid) {
		fillInfo();
		size_t nameLength = strnlen((char *)buf.get() + 0x64, 0x20);
		_name = {(char *)buf.get() + 0x64, nameLength};
	}

	ThreadInfo(int pid, int tid) : buf(new uint8_t[BUF_LENGTH]()), _name(nullptr), _pid(pid), _tid(tid) {
		fillInfo();
		size_t nameLength = strnlen((char *)buf.get() + 0x64, 0x20);
		_name = {(char *)buf.get() + 0x64, nameLength};
	}

	ThreadInfo(const ThreadInfo&) = delete;
	ThreadInfo &operator=(const ThreadInfo&) = delete;
	ThreadInfo(ThreadInfo&&) = default;
	ThreadInfo &operator=(ThreadInfo&&) = default;

	const StringView &name() const {
		return _name;
	}

	int pid() const {
		return _pid;
	}

	int tid() const {
		return _tid;
	}
};


IdArray getAllPids();
IdArray getAllTids(int pid);
void suspend(int pid);
void resume(int pid);


void read(int pid, uintptr_t addr, void *dst, size_t length);
inline UniquePtr<uint8_t[]> read(int pid, uintptr_t addr, size_t length) {
	UniquePtr<uint8_t[]> buf{new uint8_t[length]};
	read(pid, addr, buf.get(), length);
	return buf;
}

bool write(int pid, uintptr_t dst, const void *src, size_t length);

class ProcessInfoIterator {

	IdArray pids{getAllPids()};

	class iterator {

		const int *ptr;

		public:
			iterator(const int *ptr) : ptr(ptr) {}

			ProcessInfo operator*() {
				return *ptr;
			}

			iterator operator++(int) {
				iterator it = ptr;
				ptr++;
				return it;
			}

			iterator &operator++() {
				++ptr;
				return *this;
			}

			bool operator!=(const iterator &rhs) const {
				return ptr != rhs.ptr;
			}
	};

	public:
		iterator begin() const {
			return pids.begin();
		}

		iterator end() const {
			return pids.end();
		}
};

inline ProcessInfoIterator getProcesses() {
	return {};
}

class ThreadInfoIterator {

	IdArray tids;
	const int pid;

	class iterator {

		const int *ptr;
		const int pid;

		public:
			iterator(const int *ptr, int pid) : ptr(ptr), pid(pid) {}

			ThreadInfo operator*() {
				return ThreadInfo{pid, *ptr};
			}

			iterator operator++(int) {
				iterator it{ptr, pid};
				ptr++;
				return it;
			}

			iterator &operator++() {
				++ptr;
				return *this;
			}

			bool operator!=(const iterator &rhs) const {
				return ptr != rhs.ptr;
			}
	};

	public:
		ThreadInfoIterator(int pid) : tids(getAllTids(pid)), pid(pid) {}
		iterator begin() const {
			return {tids.begin(), pid};
		}

		iterator end() const {
			return {tids.end(), pid};
		}
};

inline ThreadInfoIterator getThreads(int pid) {
	return {pid};
}

} // mdbg

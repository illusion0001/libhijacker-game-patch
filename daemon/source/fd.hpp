#pragma once

#include <unistd.h>

class FileDescriptor {
	int fd;

	public:
		FileDescriptor(int fd) : fd(fd) {}
		FileDescriptor(const FileDescriptor&) = delete;
		FileDescriptor& operator=(const FileDescriptor &) = delete;
		FileDescriptor(FileDescriptor &&rhs) : fd(rhs.fd) { rhs.fd = -1; }
		FileDescriptor& operator=(FileDescriptor && rhs) {
			if (fd != -1) {
				close(fd);
			}
			fd = rhs.fd;
			rhs.fd = -1;
			return *this;
		}
		~FileDescriptor() {
			if (fd != -1) {
				close(fd);
			}
		}

		operator int() const { return fd; }
};

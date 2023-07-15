#pragma once

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

class FileDescriptor {
	static constexpr int STUPID_C_ERROR_VALUE = -1;

	protected:
		int fd;

		explicit FileDescriptor() : fd(-1) {}

	public:
		FileDescriptor(int fd) : fd(fd) {}
		FileDescriptor(const FileDescriptor&) = delete;
		FileDescriptor& operator=(const FileDescriptor &) = delete;
		FileDescriptor(FileDescriptor &&rhs) noexcept : fd(rhs.fd) { rhs.fd = -1; }
		FileDescriptor& operator=(FileDescriptor &&rhs) noexcept {
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

		bool read(void *buf, size_t size) const {
			if (::read(fd, buf, size) == STUPID_C_ERROR_VALUE) [[unlikely]] {
				int err = errno;
				printf("read failed %d %s\n", err, strerror(err));
				return false;
			}
			return true;
		}

		bool write(const void *buf, size_t size) const {
			if (::write(fd, buf, size) == STUPID_C_ERROR_VALUE) [[unlikely]] {
				int err = errno;
				printf("write failed %d %s\n", err, strerror(err));
				return false;
			}
			return true;
		}

		template <size_t N>
		bool write(const char (&buf)[N]) const {
			return write(buf, N-1);
		}

		template <size_t N>
		bool println(const char (&buf)[N]) const {
			if (!write(buf)) [[unlikely]] {
				return false;
			}
			return write("\n");
		}

		template <size_t N>
		int puts(const char (&buf)[N]) const {
			if (println(buf)) {
				return N;
			}
			return 0;
		}

		bool writeAll(const void *buf, size_t size) const {
			while (size > 0) {
				auto wrote = ::write(fd, buf, size);
				if (wrote == STUPID_C_ERROR_VALUE) [[unlikely]] {
					return false;
				}
				size -= wrote;
			}
			return true;
		}
};

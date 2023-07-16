#include "server.hpp"

bool TcpSocket::read(void *buf, size_t buflen) const noexcept {
	ssize_t nread = 0;
	while (!isClosed() && nread < buflen) {
		pollfd pfd = {.fd = fd, .events = POLLHUP | POLLRDNORM, .revents = 0};
		int res = poll(&pfd, 1, INFTIM);
		if (res == STUPID_C_ERROR_VALUE || res == 0 || pfd.revents & POLLHUP) {
			// error occured
			return false;
		}

		// we are ready to read
		auto result = ::read(fd, static_cast<uint8_t *>(buf) + nread, buflen - nread);
		if (result == STUPID_C_ERROR_VALUE) [[unlikely]] {
			int err = errno;
			printf("read failed %d %s\n", err, strerror(err));
			return false;
		}
		nread += result;
	}
	return !isClosed();
}

bool TcpSocket::write(const void *buf, size_t buflen) const noexcept {
	ssize_t wrote = 0;
	while (!isClosed() && wrote < buflen) {
		pollfd pfd = {.fd = fd, .events = POLLHUP | POLLWRNORM, .revents = 0};
		int res = poll(&pfd, 1, INFTIM);
		if (res == STUPID_C_ERROR_VALUE || res == 0 || pfd.revents & POLLHUP) {
			// error occured
			return false;
		}

		// we are ready to write
		auto result = ::write(fd, static_cast<const uint8_t *>(buf) + wrote, buflen - wrote);
		if (result == STUPID_C_ERROR_VALUE) [[unlikely]] {
			int err = errno;
			printf("write failed %d %s\n", err, strerror(err));
			return false;
		}
		wrote += result;
	}
	return !isClosed();
}

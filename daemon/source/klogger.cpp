#include <errno.h>
#include <ps5/payload_main.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stddef.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>

#include "fd.hpp"
#include "servers.hpp"
#include "util.hpp"

void KlogServer::run(TcpSocket &sock) {
	static constexpr size_t KLOG_BUF_SIZE = 256;
	static char klogbuf[KLOG_BUF_SIZE];
	FileDescriptor fd = open("/dev/klog", O_NONBLOCK, 0);
	pollfd readfds[] = {
		{.fd = fd, .events = POLLRDNORM, .revents = 0},
		{.fd = sock.getFd(), .events = POLLHUP,    .revents = 0}
	};
	while (!sock.isClosed()) {
		int res = poll(readfds, sizeof(readfds) / sizeof(pollfd), INFTIM);
		if (res == STUPID_C_ERROR_VALUE || res == 0) {
			// error occured
			return;
		}

		if (readfds[1].revents & POLLHUP) {
			// connection was closed
			return;
		}

		klogbuf[0] = '\0';
		auto nread = read(fd, klogbuf, sizeof(klogbuf));
		if (nread == STUPID_C_ERROR_VALUE) {
			// error occured
			return;
		}
		if (!sock.isClosed()) {
			sock.write(klogbuf, nread);
		}
	}
}


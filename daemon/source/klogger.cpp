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
#include "thread.hpp"
#include "util.hpp"

static constexpr int KLOGGER_PORT = 9081;

static char klogbuf[256];

static void run(int s) {
	FileDescriptor sock{s};
	FileDescriptor fd = open("/dev/klog", O_NONBLOCK, 0);
	pollfd readfds[] = {
		{.fd = fd, .events = POLLRDNORM, .revents = 0},
		{.fd =  s, .events = POLLHUP,    .revents = 0}
	};
	pollfd writefd = {.fd =  s, .events = POLLHUP | POLLWRNORM, .revents = 0};
	while (true) {
		int res = poll(readfds, sizeof(readfds) / sizeof(pollfd), INFTIM);
		if (res == -1 || res == 0) {
			// error occured
			return;
		}

		if (readfds[1].revents & POLLHUP) {
			// connection was closed
			return;
		}

		klogbuf[0] = '\0';
		auto remaining = read(fd, klogbuf, sizeof(klogbuf));
		if (remaining == -1) {
			// error occured
			return;
		}
		while (remaining > 0) {
			writefd.revents = 0;
			if (poll(&writefd, 1, INFTIM) == -1) {
				return;
			}
			if (writefd.revents & POLLHUP) {
				// connection was closed
				return;
			}
			auto sent = write(sock, klogbuf, remaining);
			if (sent <= 0) {
				return;
			}
			remaining -= sent;
		}
	}
}

int runKlogger(void *unused) {
	FileDescriptor sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		return 0;
	}

	int value = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) < 0) {
		return 0;
	}

	struct sockaddr_in server_addr{0, AF_INET, htons(KLOGGER_PORT), {}, {}};

	if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
		return 0;
	}

	if (listen(sock, 1) != 0) {
		return 0;
	}

	struct sockaddr client_addr{};
	socklen_t addr_len = sizeof(client_addr);
	while (true) {
		int fd = accept(sock, &client_addr, &addr_len);
		if (fd != -1) {
			run(fd);
		}
	}
}


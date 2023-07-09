#include <netinet/in.h>
#include <poll.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "fd.hpp"
#include "thread.hpp"

extern int runKlogger(void *unused);
extern int runElfServer(void *unused);
extern int runCommandProcessor(void *unused);

static constexpr int LOGGER_PORT = 9071;
static constexpr int STDOUT = 1;
static constexpr int STDERR = 2;

static int initStdout() {
	socklen_t addr_len;
	{
		FileDescriptor sock = socket(AF_INET, SOCK_STREAM, 0);

		if (!sock) {
			return -1;
		}

		int value = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) < 0) {
			return -1;
		}

		struct sockaddr_in server_addr{0, AF_INET, htons(LOGGER_PORT), {}, {}};

		if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
			return -1;
		}

		if (listen(sock, 1) != 0) {
			return -1;
		}

		struct sockaddr client_addr{};
		addr_len = sizeof(client_addr);
		int conn = accept(sock, &client_addr, &addr_len);
		if (conn != -1) {
			return conn;
		}
	}
	return -1;
}

class Stdout {
	int fd = -1;

	public:
		explicit Stdout() : fd(initStdout()) {
			if (fd != -1) {
				dup2(fd, STDOUT);
				dup2(fd, STDERR);
				close(fd);
			}
		}
		~Stdout() {
			if (fd != -1) {
				close(STDOUT);
				close(STDERR);
			}
		}

		operator int() const { return fd; }
};

static int runStdoutServer(void *unused) {
	while (true) {
		Stdout out{};
		pollfd fpd = {.fd =  out, .events = POLLHUP, .revents = 0};
		if (poll(&fpd, 1, INFTIM) == -1) {
			return 0;
		}
	}
}

int main() {
	JThread stdoutThread{runStdoutServer};
	JThread kloggerThread{runKlogger};
	//JThread elfServerThread{runElfServer};
	JThread commandThread{runCommandProcessor};

	// TODO add elf loader with options for process name and type (daemon/game)
	// add whatever other crap people may want
	return 0;
}

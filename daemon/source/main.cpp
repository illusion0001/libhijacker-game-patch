#include <netinet/in.h>
#include <poll.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "fd.hpp"
#include "servers.hpp"

extern int runKlogger(void *unused);
extern int runElfServer(void *unused);
extern int runCommandProcessor(void *unused);

static constexpr int LOGGER_PORT = 9071;
static constexpr int STDOUT = 1;
static constexpr int STDERR = 2;

static int initStdout() {
	FileDescriptor sock = socket(AF_INET, SOCK_STREAM, 0);

	if (!sock) {
		return -1;
	}

	int value = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) < 0) {
		return -1;
	}

	struct sockaddr_in server_addr{0, AF_INET, htons(LOGGER_PORT), {}, {}};

	if (bind(sock, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) != 0) {
		return -1;
	}

	if (listen(sock, 1) != 0) {
		return -1;
	}

	struct sockaddr client_addr{};
	socklen_t addr_len = sizeof(client_addr);
	int conn = accept(sock, &client_addr, &addr_len);
	if (conn != -1) {
		return conn;
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
		Stdout(const Stdout&) = delete;
		Stdout &operator=(const Stdout&) = delete;
		Stdout(Stdout&&) = delete;
		Stdout &operator=(Stdout&&) = delete;
		~Stdout() {
			if (fd != -1) {
				close(STDOUT);
				close(STDERR);
			}
		}

		operator int() const { return fd; }
};


extern void makenewapp();

void StdoutServer::run(TcpSocket &sock) {
	bool madeApp = false;
	while (true) {
		Stdout out{};
		if (!madeApp) {
			puts("making app");
			makenewapp();
			madeApp = true;
		}
		pollfd fpd = {.fd =  out, .events = POLLHUP, .revents = 0};
		if (poll(&fpd, 1, INFTIM) == -1) {
			return;
		}
	}
}

int main() {
	StdoutServer stdoutServer{};
	KlogServer klogServer{};
	ElfServer elfServer{};
	CommandServer commandServer{};

	// finishes on command
	commandServer.join();
	elfServer.stop();
	stdoutServer.stop();
	klogServer.stop();

	// TODO add elf loader with options for process name and type (daemon/game)
	// add whatever other crap people may want
	return 0;
}

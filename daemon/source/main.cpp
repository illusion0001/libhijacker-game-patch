#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "fd.hpp"
#include "thread.hpp"

extern int runKlogger(void *unused);
extern int runElfServer(void *unused);

static constexpr int COMMAND_PORT = 9028;
static constexpr int8_t ACTIVE_CMD = 0;
static constexpr int8_t INVALID_CMD = -1;

static int runCommandProcessor(void *unused) {
	// for now this is solely for checking if we are running
	FileDescriptor sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		return 0;
	}

	int value = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) < 0) {
		return 0;
	}

	struct sockaddr_in server_addr{0, AF_INET, htons(COMMAND_PORT), {}, {}};

	if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
		return 0;
	}

	if (listen(sock, 1) != 0) {
		return 0;
	}

	struct sockaddr client_addr{};
	socklen_t addr_len = sizeof(client_addr);
	while (true) {
		FileDescriptor fd = accept(sock, &client_addr, &addr_len);
		if (fd != -1) {
			int8_t cmd = INVALID_CMD;
			if (read(fd, &cmd, sizeof(cmd)) != sizeof(cmd)) {
				puts("failed to read command");
				continue;
			}
			if (cmd == ACTIVE_CMD) {
				if (write(fd, &cmd, sizeof(cmd)) != sizeof(cmd)) {
					puts("failed to write command response");
				}
			} else {
				cmd = INVALID_CMD;
				if (write(fd, &cmd, sizeof(cmd)) != sizeof(cmd)) {
					puts("failed to write command response");
				}
			}
		}
	}
	return 0;
}

extern "C" void sceKernelDebugOutText(const char *);

int main() {
	sceKernelDebugOutText("Homebrew daemon started\n");
	JThread kloggerThread{runKlogger, nullptr};
	JThread elfServerThread{runElfServer, nullptr};
	JThread commandThread{runCommandProcessor, nullptr};

	// TODO add elf loader with options for process name and type (daemon/game)
	// add whatever other crap people may want
	return 0;
}

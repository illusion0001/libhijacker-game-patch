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
extern void makenewapp();

void AbortServer::run(TcpSocket &sock) {
	// any connection signals to shutdown the daemon
	puts("abort signal received");
	sock.close();
}

int main() {
	puts("daemon entered");
	puts("making app");
	makenewapp();
	AbortServer abortServer{};
	KlogServer klogServer{};
	ElfServer elfServer{};
	CommandServer commandServer{};

	// finishes on connect
	abortServer.join();
	puts("abort thread finished");
	commandServer.stop();
	puts("command server done");
	puts("stopping elf server");
	elfServer.stop();
	puts("elf server done");
	puts("stopping klog server");
	klogServer.stop();
	puts("klog server done");

	// TODO add elf loader with options for process name and type (daemon/game)
	// add whatever other crap people may want
	return 0;
}

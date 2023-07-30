#include <netinet/in.h>
#include <poll.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "fd.hpp"
#include "servers.hpp"
#include "notify.hpp"

#include <pthread.h>
#include "game_patch_thread.hpp"

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
	printf_notification("libhijacker daemon started successfully.\n"
		"Original project: https://github.com/astrelsky/libhijacker");
	printf_notification("libhijacker - astrelsky\n"
		"Fork with Game Patch Support - illusion\n"
		"Check Readme for supported titles and it's versions.");

	// remove this when it's possible to load elf into games at boot
	pthread_t game_patch_thread_id = nullptr;
	pthread_create(&game_patch_thread_id, nullptr, GamePatch_Thread, nullptr);

	g_game_patch_thread_running = true;
	AbortServer abortServer{};
	KlogServer klogServer{};
	ElfServer elfServer{};
	CommandServer commandServer{};

	// FIXME: ewww
	abortServer.TcpServer::run();
	klogServer.TcpServer::run();
	elfServer.TcpServer::run();
	commandServer.TcpServer::run();

	// finishes on connect
	abortServer.join();
	puts("abort thread finished");
	g_game_patch_thread_running = false;
	puts("g_game_patch_thread_running = false");
	commandServer.stop();
	puts("command server done");
	puts("stopping elf server");
	elfServer.stop();
	puts("elf server done");
	puts("stopping klog server");
	klogServer.stop();
	puts("klog server done");
	pthread_join(game_patch_thread_id, nullptr);
	puts("game patch thread finished");

	// TODO add elf loader with options for process name and type (daemon/game)
	// add whatever other crap people may want
	return 0;
}

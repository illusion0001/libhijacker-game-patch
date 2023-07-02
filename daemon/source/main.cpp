#include "thread.hpp"

extern int runKlogger(void *unused);

int main() {
	JThread kloggerThread{runKlogger, nullptr};

	// TODO add elf loader with options for process name and type (daemon/game)
	// add whatever other crap people may want
	return 0;
}

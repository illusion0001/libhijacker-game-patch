
static constexpr int AF_INET6 = 28;
static constexpr int SOCK_DGRAM = 2;
static constexpr int IPPROTO_UDP = 17;
static constexpr int IPPROTO_IPV6 = 41;
static constexpr int IPV6_TCLASS = 61;
static constexpr int IPV6_2292PKTOPTIONS = 25;
static constexpr int IPV6_PKTINFO = 46;

struct Args {
	struct Result {
		int state;
		int err;
	} result;
	int *__restrict files;
	int (*usleep)(unsigned int);
	int (*socket)(int, int, int);
	int (*pipe)(int*);
	int (*setsockopt)(int sockfd, int level, int optname, const void *optval, int optlen);
	int *(*errno)();
};

[[noreturn]] void setup(Args *__restrict args) {

	args->files[0] = args->socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	args->files[1] = args->socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (args->pipe(args->files + 2) == -1) [[unlikely]] {
		args->result.err = *args->errno();
	}

	volatile unsigned int buf[6];
	buf[0] = sizeof(buf) - sizeof(int);
	buf[1] = IPPROTO_IPV6;
	buf[2] = IPV6_TCLASS;
	buf[3] = 0;
	buf[4] = 0;
	buf[5] = 0;

	args->setsockopt(args->files[0], IPPROTO_IPV6, IPV6_2292PKTOPTIONS, (void*)buf, sizeof(buf));

	buf[0] = 0;
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 0;
	buf[4] = 0;
	buf[5] = 0;
	args->setsockopt(args->files[1], IPPROTO_IPV6, IPV6_PKTINFO, (void*)buf, sizeof(buf) - sizeof(int));

	args->result.state = 1;

	for (;;) {
		// sleep for one second intervals
		// nothing too crazy so that redis can properly resume
		args->usleep(1000000);
	}
}

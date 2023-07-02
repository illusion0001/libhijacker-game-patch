#pragma once

extern "C" {

#include <pthread.h>

typedef pthread_t thrd_t;
typedef int (*thrd_start_t)(void*);

extern int _Thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
extern thrd_t _Thrd_current(void);
extern int _Thrd_equal(thrd_t lhs, thrd_t rhs);
[[noreturn]] extern void _Thrd_exit(int res);
extern thrd_t _Thrd_current(void);
extern int _Thrd_join(thrd_t thr, int *res);
extern void _Thrd_yield(void);
extern int _Thrd_detach(thrd_t thr);

}

namespace std {
	[[noreturn]] extern void terminate();
}

class JThread;

class Thread {

	friend class JThread;
	thrd_t id;

	public:
		Thread() = default;

		Thread(thrd_start_t fun, void *args) : id{0} {
			_Thrd_create(&id,  fun, args);
		}

		~Thread() {
			if (id != 0) {
				std::terminate();
			}
		}

		thrd_t get_id() const { return id; }
		bool joinable() const { return id != 0; }
		void join() {
			if (joinable()) {
				_Thrd_join(id, nullptr);
				id = 0;
			}
		}
		void detach() {
			if (joinable()) {
				thrd_t tmp = id;
				id = 0;
				_Thrd_detach(tmp);
			}
		}
};


class JThread : public Thread {

	public:
		JThread(thrd_start_t fun, void *args) : Thread{fun, args} {}
		~JThread() {
			join();
		}
};

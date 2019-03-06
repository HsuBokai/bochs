#include "thread_pid.h"
#include "semaphore.h"

semaphore_t next_pid_sema;

static void next_pid_acquire(void)
{
	sema_down(&next_pid_sema);
}

static void next_pid_release(void)
{
	sema_up(&next_pid_sema);
}

void thread_pid_init(void)
{
	sema_init(&next_pid_sema);
}

pid_t allocate_pid(void)
{
	static pid_t next_pid = 0;
	next_pid_acquire();
	next_pid++;
	next_pid_release();
	return next_pid;
}

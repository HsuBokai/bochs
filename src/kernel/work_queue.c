#include "work_queue.h"
#include "schedule.h"
#include "mm.h"

static void work_queue_thread(void *arg);

static void work_queue_thread(void *arg)
{
	work_queue_t *self = (work_queue_t*) arg;
	thread_stack_t *th_stack = NULL;
	thread_func *function = NULL;
	void *func_arg = NULL;

	while(1) {
		if (0 == list_size(&(self->queue))) {
			schedule();
		} else {
			self->curr_work = elem2struct(work_t, general_tag, list_pop_head(&(self->queue)));
			thread_start(self->curr_work);
			th_stack = (thread_stack_t*) self->curr_work->stack_ptr;
			function = th_stack->function;
			func_arg = th_stack->func_arg;
			function(func_arg);
		}
	}
}

void work_queue_init(work_queue_t *self)
{
	self->curr_work = NULL;
	list_init(&(self->queue));
}

void work_queue_ready(work_queue_t *self)
{
	thread_t *th = (thread_t*) malloc_page(PF_KERNEL, 1);
	thread_init(th, "thread work", 31);
	thread_func_setup(th, work_queue_thread, self);
	thread_ready(th);
}

void work_queue_add(work_queue_t *self, thread_func *function, void *func_arg)
{
}

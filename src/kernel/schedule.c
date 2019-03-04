#include "schedule.h"
#include "list.h"
#include "thread.h"
#include "debug.h"
#include "switch.h"
#include "interrupt.h"

list_t thread_ready_list;
list_t thread_all_list;

void schedule(void)
{
	thread_t *curr = NULL, *next = NULL;
	list_elem_t *next_tag = NULL;

	ASSERT(!is_interrupt_enabled());

	curr = running_thread();

	if (TASK_RUNNING == curr->status) {
		list_insert_tail(&thread_ready_list, &(curr->general_tag));
		curr->status = TASK_READY;
		curr->ticks = curr->priority;
	}

	next_tag = list_pop_head(&thread_ready_list);
	next = elem2struct(thread_t, general_tag, next_tag);
	next->status = TASK_RUNNING;

	thread_activate(next);

	switch_to(curr, next);
}

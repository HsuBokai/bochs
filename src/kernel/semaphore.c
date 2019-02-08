#include "semaphore.h"
#include "list.h"
#include "thread.h"
#include "debug.h"
#include "interrupt.h"
#include "schedule.h"

void sema_init(semaphore_t *self)
{
	self->resource = 1;
	list_init(&(self->waiting_list));
}

void sema_up(semaphore_t *self)
{
	thread_t *wakeup = NULL;
	list_elem_t *wakeup_tag = NULL;
	int org_interrupt_enabled = is_interrupt_enabled();

	ASSERT(0 == self->resource);

	if (org_interrupt_enabled) INTR_DISABLE;

	if (0 < list_size(&(self->waiting_list))) {
		wakeup_tag = list_pop_head(&(self->waiting_list));
		wakeup = elem2struct(thread_t, general_tag, wakeup_tag);

		/* thread_unblock(wakeup); */
		ASSERT(TASK_BLOCKED == wakeup->status)
		list_insert_tail(&thread_ready_list, &(wakeup->general_tag));
		wakeup->status = TASK_READY;
	}

	self->resource++;

	if (org_interrupt_enabled) INTR_ENABLE;
}

void sema_down(semaphore_t *self)
{
	thread_t *curr = NULL;
	int org_interrupt_enabled = is_interrupt_enabled();

	if (org_interrupt_enabled) INTR_DISABLE;

	while (0 == self->resource) {
		curr = running_thread();
		list_insert_tail(&(self->waiting_list), &(curr->general_tag));

		/* thread_block(); */
		curr->status = TASK_BLOCKED;
		schedule();
	}

	self->resource--;

	if (org_interrupt_enabled) INTR_ENABLE;
}

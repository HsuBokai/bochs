#include "thread.h"
#include "debug.h"
#include "mem.h"
#include "mm.h"
#include "schedule.h"
#include "interrupt.h"

static void thread_start_helper(thread_func* function, void* func_arg)
{
	INTR_ENABLE;
	function(func_arg);
}

void thread_init(thread_t *self, int8_t *name, uint8_t priority)
{
	uint32_t thread_stack_start = (uint32_t)self + PG_SIZE, i = 0;

	ASSERT(NULL != name);

	self->stack_ptr = (uint32_t*) thread_stack_start;
	self->status = TASK_READY;
	self->priority = priority;

	for (; name[i]; ++i) self->name[i] = name[i];

	self->ticks = priority;
	self->elapsed_ticks = 0;
	list_elem_init(&(self->general_tag));
	list_elem_init(&(self->all_list_tag));
	self->pg_base_ptr = NULL;

	self->stack_magic = 0x12345678;

	list_insert_tail(&thread_all_list, &(self->all_list_tag));
}

void thread_func_setup(thread_t *self, thread_func *function, void *func_arg)
{
	thread_stack_t *th_stack = NULL;

	self->stack_ptr -= sizeof(intr_stack_t);
	self->stack_ptr -= sizeof(thread_stack_t);
	th_stack = (thread_stack_t*) self->stack_ptr;
	th_stack->eip = thread_start_helper;
	th_stack->function = function;
	th_stack->func_arg = func_arg;
}

void thread_start(thread_t *self)
{
	self->status = TASK_RUNNING;
	asm volatile ("			\
		movl %0, %%esp;		\
		pop %%ebp;		\
		pop %%ebx;		\
		pop %%edi;		\
		pop %%esi;		\
		ret;			\
	": : "g" (self->stack_ptr) : "memory");
}

void thread_ready(thread_t *self)
{
	self->status = TASK_READY;
	list_insert_tail(&thread_ready_list, &(self->general_tag));
}

thread_t* running_thread(void)
{
	uint32_t esp;
	GET_STACK_PTR(esp);
	return (thread_t*)(esp & 0xfffff000);
}

void thread_block(void)
{
	thread_t *curr = NULL;
	int org_interrupt_enabled = is_interrupt_enabled();

	curr = running_thread();

	ASSERT(TASK_RUNNING == curr->status);

	if (org_interrupt_enabled) INTR_DISABLE;
	curr->status = TASK_BLOCKED;
	schedule();
	if (org_interrupt_enabled) INTR_ENABLE;
}

void thread_unblock(thread_t *th)
{
	int org_interrupt_enabled = is_interrupt_enabled();

	ASSERT(TASK_BLOCKED == th->status)

	if (org_interrupt_enabled) INTR_DISABLE;
	list_insert_tail(&thread_ready_list, &(th->general_tag));
	th->status = TASK_READY;
	if (org_interrupt_enabled) INTR_ENABLE;
}

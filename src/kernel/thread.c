#include "thread.h"
#include "debug.h"
#include "mem.h"
#include "mm.h"
#include "schedule.h"
#include "interrupt.h"
#include "tss.h"
#include "syscall.h"

thread_t *idle_thread;

static void thread_start_helper(thread_func* function, void* func_arg)
{
	INTR_ENABLE;
	function(func_arg);
}

static void thread_activate_page(thread_t *self)
{
	uint32_t page_base_phy_addr = IS_KERNEL_THREAD(self) ? 0x100000 : self->page_base_phy_addr;
	asm volatile("movl %0, %%cr3" : : "r" (page_base_phy_addr) : "memory");
}

void thread_init(thread_t *self, int8_t *name, uint8_t priority)
{
	uint32_t thread_stack_start = (uint32_t)self + PG_SIZE, i = 0;

	ASSERT(NULL != name);

	self->stack_ptr = (uint32_t*) thread_stack_start;
	self->pid = allocate_pid();
	self->status = TASK_READY;
	self->priority = priority;

	for (; name[i]; ++i) self->name[i] = name[i];

	self->ticks = priority;
	self->elapsed_ticks = 0;
	list_elem_init(&(self->general_tag));
	list_elem_init(&(self->all_list_tag));

	self->page_base_phy_addr = 0;
	vaddr_init(&(self->user_proc_vaddr), U_HEAP_START, U_HEAP_LIMIT);

	self->stack_magic = 0x12345678;

	list_insert_tail(&thread_all_list, &(self->all_list_tag));
}

void thread_func_setup(thread_t *self, thread_func *function, void *func_arg)
{
	thread_stack_t *th_stack = NULL;

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

void thread_activate(thread_t *self)
{
	ASSERT(NULL != self);
	if (!IS_KERNEL_THREAD(self)) {
		update_tss_esp(self);
	}
	thread_activate_page(self);
}

thread_t* running_thread(void)
{
	uint32_t esp;
	GET_STACK_PTR(esp);
	return (thread_t*)(esp & 0xfffff000);
}

static void idle(void *arg)
{
	while(1) {
		thread_block();
		asm volatile("sti; hlt;" : : : "memory");
	}
}

void become_idle_thread(void)
{
	idle_thread = (thread_t*) malloc_page(PF_KERNEL, 1);
	ASSERT(NULL != idle_thread);

	thread_init(idle_thread, "idle thread", 1);
	thread_func_setup(idle_thread, idle, NULL);
	thread_start(idle_thread);
}

uint32_t sys_thread_yield(void)
{
	thread_t *curr = running_thread();

	list_insert_tail(&thread_ready_list, &(curr->general_tag));
	curr->status = TASK_READY;
	schedule();

	return (uint32_t)SYS_THREAD_YIELD;
}

uint32_t sys_thread_block(void)
{
	thread_t *curr = running_thread();

	curr->status = TASK_BLOCKED;
	schedule();

	return (uint32_t)SYS_THREAD_BLOCK;
}

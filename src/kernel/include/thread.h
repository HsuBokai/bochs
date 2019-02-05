#ifndef __THREAD_H
#define __THREAD_H
#include "stdint.h"
#include "list.h"

typedef void thread_func(void*);

#define GET_STACK_PTR(ESP_VAR) \
	asm volatile("mov %%esp, %0" : "=g" (ESP_VAR))

typedef enum thread_status {
	TASK_RUNNING,
	TASK_READY,
	TASK_BLOCKED,
	TASK_WAITING,
	TASK_HANGING,
	TASK_DIED
} thread_status_t;

typedef struct intr_stack {
	/* push %1; in intr.S */
	uint32_t vec_no;

	/* pushad; in intr.S */
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp_dummy;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;

	/* push gs; in intr.S */
	uint32_t gs;
	/* push fs; in intr.S */
	uint32_t fs;
	/* push es; in intr.S */
	uint32_t es;
	/* push ds; in intr.S */
	uint32_t ds;

	/* Timer Interrupt (vec_no: 0x20) with NO_ERROR_CODE */
	//uint32_t err_code;

	/* Auto Push Registers by CPU Handle Interrupts */
	void (*eip) (void);
	uint32_t cs;
	uint32_t eflags;
	void* esp;
	uint32_t ss;
} intr_stack_t;

typedef struct thread_stack {
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edi;
	uint32_t esi;

	void (*eip) (thread_func* func, void* func_arg);
	void (*unused_ret_addr);
	thread_func* function;
	void* func_arg;
} thread_stack_t;

typedef struct thread {
	uint32_t* stack_ptr;
	thread_status_t status;
	uint8_t priority;
	int8_t name[19];

	uint8_t ticks;
	uint32_t elapsed_ticks;
	list_elem_t general_tag;
	list_elem_t all_list_tag;
	uint32_t* pg_base_ptr;

	uint32_t stack_magic;
} thread_t;

void thread_init(thread_t *self, int8_t *name, uint8_t priority);
void thread_func_setup(thread_t *self, thread_func *function, void *func_arg);
void thread_start(thread_t *self);
void thread_ready(thread_t *self);

thread_t* running_thread(void);
void make_main_thread(void);

#endif /* __THREAD_H */

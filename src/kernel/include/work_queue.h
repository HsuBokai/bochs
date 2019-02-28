#ifndef __WORK_QUEUE_H
#define __WORK_QUEUE_H
#include "stdint.h"
#include "list.h"

typedef void thread_func(void*);

typedef struct work {
	thread_func* function;
	void* func_arg;
	list_elem_t general_tag;
} work_t

typedef struct work_queue {
	work_t* curr_work;
	list_t queue;
} work_queue_t;

void work_queue_init(work_queue_t *self);
void work_queue_ready(work_queue_t *self);

#endif /* __WORK_QUEUE_H */

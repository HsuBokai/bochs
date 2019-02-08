#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H
#include "stdint.h"
#include "list.h"

typedef struct semaphore {
	uint8_t resource;
	list_t waiting_list;
} semaphore_t;

void sema_init(semaphore_t *self);
void sema_up(semaphore_t *self);
void sema_down(semaphore_t *self);

#endif /* __SEMAPHORE_H */

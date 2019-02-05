#ifndef __LIB_LIST_H
#define __LIB_LIST_H
#include "stdint.h"

typedef struct list {
	struct list* prev;
	struct list* next;
} list_t;

void list_init(list_t *self);
void list_insert_before(list_t *elem, list_t *new_elem);
void list_insert_after(list_t *elem, list_t *new_elem);
int elem_find(list_t *self, list_t *elem);

#endif /* __LIB_LIST_H */

#ifndef __LIB_LIST_H
#define __LIB_LIST_H
#include "stdint.h"

typedef struct list_elem {
	struct list_elem* prev;
	struct list_elem* next;
} list_elem_t;


void list_elem_init(list_elem_t *self);
void list_elem_insert_before(list_elem_t *self, list_elem_t *new_elem);
void list_elem_insert_after(list_elem_t *self, list_elem_t *new_elem);
void list_elem_remove(list_elem_t *self);

typedef struct list {
	list_elem_t *head;
	list_elem_t *tail;
} list_t;

void list_init(list_t *self);
void list_insert_head(list_t *self, list_elem_t *elem);
void list_insert_tail(list_t *self, list_elem_t *elem);
int list_find_elem(list_t *self, list_elem_t *elem);
list_elem_t* list_pop_head(list_t *self);
list_elem_t* list_pop_tail(list_t *self);

#endif /* __LIB_LIST_H */

#include "list.h"
#include "debug.h"

void list_elem_init(list_elem_t *self)
{
	self->next = NULL;
	self->prev = NULL;
}

void list_elem_insert_before(list_elem_t *self, list_elem_t *new_elem)
{
	ASSERT(NULL != self);
	ASSERT(NULL != new_elem);

	new_elem->prev = self->prev;
	new_elem->next = self;
	self->prev = new_elem;
	if (NULL != new_elem->prev)
		new_elem->prev->next = new_elem;
}

void list_elem_insert_after(list_elem_t *self, list_elem_t *new_elem)
{
	ASSERT(NULL != self);
	ASSERT(NULL != new_elem);

	new_elem->prev = self;
	new_elem->next = self->next;
	self->next = new_elem;
	if (NULL != new_elem->next)
		new_elem->next->prev = new_elem;
}

void list_init(list_t *self)
{
	self->head = NULL;
	self->tail = NULL;
}

void list_insert_head(list_t *self, list_elem_t *elem)
{
	if (NULL != self->head)
		list_elem_insert_before(self->head, elem);

	self->head = elem;

	if (NULL == self->tail)
		self->tail = elem;
}

void list_insert_tail(list_t *self, list_elem_t *elem)
{
	if (NULL != self->tail)
		list_elem_insert_after(self->tail, elem);

	self->tail = elem;

	if (NULL == self->head)
		self->head = elem;
}

int list_find_elem(list_t *self, list_elem_t *elem)
{
	int ret = -1;
	list_elem_t *check = self->head;

	while (NULL != check) {
		if (elem == check) {
			ret = 1;
			goto Found;
		}
		check = check->next;
	}

	ret = 0;
Found:
	return ret;
}

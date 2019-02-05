#include "list.h"
#include "debug.h"

void list_init(list_t *self)
{
	self->next = NULL;
	self->prev = NULL;
}

void list_insert_before(list_t *elem, list_t *new_elem)
{
	ASSERT(NULL != elem);
	ASSERT(NULL != new_elem);

	new_elem->prev = elem->prev;
	new_elem->next = elem;
	elem->prev = new_elem;
	if (NULL != new_elem->prev)
		new_elem->prev->next = new_elem;
}

void list_insert_after(list_t *elem, list_t *new_elem)
{
	ASSERT(NULL != elem);
	ASSERT(NULL != new_elem);

	new_elem->prev = elem;
	new_elem->next = elem->next;
	elem->next = new_elem;
	if (NULL != new_elem->next)
		new_elem->next->prev = new_elem;
}

int elem_find(list_t *self, list_t *elem)
{
	if (NULL == self)
		return 0;

	if (self == elem)
		return 1;

	return elem_find(self->next, elem);
}

#include "vaddr.h"
#include "mm.h"

void vaddr_init(virtual_addr_t *self, uint32_t start, uint32_t limit)
{
	self->vaddr_start = start;
	self->vaddr_limit = limit;
}

int vaddr_available(virtual_addr_t *self, uint32_t pg_count)
{
	return (self->vaddr_start + PG_SIZE * pg_count > self->vaddr_limit) ? 0 : 1;
}

uint32_t vaddr_get(virtual_addr_t *self, uint32_t pg_count)
{
	uint32_t ret = 0;

	if (0 == vaddr_available(self, pg_count)) {
		goto END;
	}

	ret = self->vaddr_start;
	self->vaddr_start += (pg_count * PG_SIZE);
END:
	return ret;
}

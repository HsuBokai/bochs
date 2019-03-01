#ifndef __VIRTUAL_MEMORY_H
#define __VIRTUAL_MEMORY_H
#include "stdint.h"

typedef struct virtual_addr {
	uint32_t vaddr_start;
	uint32_t vaddr_limit;
} virtual_addr_t;

void vaddr_init(virtual_addr_t *self, uint32_t start, uint32_t limit);
int vaddr_available(virtual_addr_t *self, uint32_t pg_count);
uint32_t vaddr_get(virtual_addr_t *self, uint32_t pg_count);

#endif /* __VIRTUAL_MEMORY_H */

#ifndef __MEMORY_MANAGEMENT_H
#define __MEMORY_MANAGEMENT_H
#include "stdint.h"

#define PG_SIZE			4096
#define K_HEAP_START		0xc0100000
#define K_HEAP_LIMIT		0xffffffff
#define TOTAL_PHY_MEM_SIZE	32 * 1024 * 1024
#define K_BITMAP_BYTES_LEN	(TOTAL_PHY_MEM_SIZE / PG_SIZE / 8)

#define PG_P_0		0
#define PG_P_1		1
#define	PG_RW_R		(0 << 1)
#define	PG_RW_W		(1 << 1)
#define PG_US_S		(0 << 2)
#define	PG_US_U		(1 << 2)

typedef enum pool_flags {
	PF_KERNEL = 1
} pool_flags_t;

typedef struct mem_pool mem_pool_t;

void mem_init(void);
void* malloc_page(pool_flags_t pf, uint32_t pg_count);

#endif /* __MEMORY_MANAGEMENT_H */

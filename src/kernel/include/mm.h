#ifndef __MEMORY_MANAGEMENT_H
#define __MEMORY_MANAGEMENT_H
#include "stdint.h"
#include "bitmap.h"

#define PG_SIZE			4096
#define K_HEAP_START		0xc0100000
#define K_HEAP_LIMIT		0xffffffff
#define U_HEAP_START		0x08048000
#define U_HEAP_LIMIT		(0xc0000000 - PG_SIZE - 1)
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

typedef struct mem_pool {
	bitmap_t pool_bitmap;
	uint32_t phy_mem_offset;
	uint8_t bitmap_array[K_BITMAP_BYTES_LEN];
} mem_pool_t;

void page_table_add(uint32_t page_vir_addr, uint32_t page_phy_addr);
void mem_init(void);
void* malloc_page(pool_flags_t pf, uint32_t pg_count);
uint32_t addr_v2p(uint32_t vaddr);

#endif /* __MEMORY_MANAGEMENT_H */

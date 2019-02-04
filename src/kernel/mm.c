#include "mm.h"
#include "bitmap.h"
#include "print.h"
#include "debug.h"

#define PDE_IDX(addr) ((addr & 0xffc00000) >> 22)
#define PTE_IDX(addr) ((addr & 0x003ff000) >> 12)

typedef struct mem_pool {
	bitmap_t pool_bitmap;
	uint32_t phy_mem_offset;
	uint8_t bitmap_array[K_BITMAP_BYTES_LEN];
} mem_pool_t;

mem_pool_t kernel_pool;
uint32_t kernel_vaddr_ptr = K_HEAP_START;

static void mem_pool_init(mem_pool_t *self, const uint32_t phy_mem_offset, const uint32_t phy_mem_size)
{
	uint32_t bits_len  = phy_mem_size / PG_SIZE;
	bitmap_init(&(self->pool_bitmap), bits_len, self->bitmap_array);
	self->phy_mem_offset = phy_mem_offset;
}

static int mem_pool_palloc(mem_pool_t *self, uint32_t *phy_mem_start)
{
	int ret = -1;
	uint32_t bit_offset = 0;
	if ( 0 != bitmap_scan(&(self->pool_bitmap), 1, &bit_offset))
		goto FAIL;

	*phy_mem_start = self->phy_mem_offset + bit_offset * PG_SIZE;

	bitmap_set(&(self->pool_bitmap), bit_offset, 1);

	ret = 0;
FAIL:
	return ret;
}

static uint32_t* pte_ptr(uint32_t vaddr)
{
	return (uint32_t*)(0xffc00000 + ((vaddr & 0xffc00000) >> 10) + PTE_IDX(vaddr) * 4);
}

static uint32_t* pde_ptr(uint32_t vaddr)
{
	return (uint32_t*)(0xfffff000 + PDE_IDX(vaddr) * 4);
}

static void page_table_add(uint32_t page_vir_addr, uint32_t page_phy_addr)
{
	uint32_t *pde = NULL, *pte = NULL;
	uint32_t new_pte = 0;

	ASSERT(0 == (page_phy_addr & 0x00000fff));

	pde = pde_ptr(page_vir_addr);
	if (0 == (*pde & 0x00000001)) {
		if (0 != mem_pool_palloc(&kernel_pool, &new_pte)) {
			ASSERT(0);
		}
		*pde = (new_pte | PG_US_U | PG_RW_W | PG_P_1);
		memset((uint8_t*)new_pte, 0, PG_SIZE);
	}

	pte = pte_ptr(page_vir_addr);
	*pte = (page_phy_addr | PG_US_U | PG_RW_W | PG_P_1);
}

static int check_vir_addr_available(uint32_t pg_count)
{
	return (kernel_vaddr_ptr + PG_SIZE * pg_count > K_HEAP_LIMIT) ? 0 : 1;
}

void mem_init(void)
{
	uint32_t used_mem = 0, all_mem = 0;
	put_str("mem_init start\n");

	used_mem = 0x100000 + PG_SIZE * 2; //1MB + PDE * 1 + PTE * 1
	all_mem = TOTAL_PHY_MEM_SIZE;
	mem_pool_init(&kernel_pool, used_mem, all_mem - used_mem);

	put_str("mem_init done\n");
}

void* malloc_page(pool_flags_t pf, uint32_t pg_count)
{
	mem_pool_t *mem_pool = NULL;
	uint32_t vir_mem_start = 0, phy_mem_start = 0, bit_offset = 0, i = 0;

	mem_pool = (pf & PF_KERNEL) ? &kernel_pool : NULL;

	if (NULL == mem_pool)
		goto FAIL;

	if (0 == check_vir_addr_available(pg_count))
		goto FAIL;

	if (0 != bitmap_scan(&(mem_pool->pool_bitmap), pg_count, &bit_offset))
		goto FAIL;

	put_int((uint8_t)bit_offset);

	phy_mem_start = mem_pool->phy_mem_offset + bit_offset * PG_SIZE;
	vir_mem_start = kernel_vaddr_ptr;

	for(i = 0; i < pg_count; ++i) {
		bitmap_set(&(mem_pool->pool_bitmap), bit_offset + i, 1);
		page_table_add(kernel_vaddr_ptr,  phy_mem_start);
		kernel_vaddr_ptr += PG_SIZE;
		phy_mem_start += PG_SIZE;
	}

	return (void*)vir_mem_start;

FAIL:
	return NULL;
}

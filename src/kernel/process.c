#include "process.h"
#include "debug.h"
#include "global.h"
#include "mm.h"
#include "bitmap.h"
#include "thread.h"

extern mem_pool_t kernel_pool;

static uint32_t malloc_proc_stack(void)
{
	mem_pool_t *mem_pool = &kernel_pool;
	thread_t *curr = running_thread();
	uint32_t vir_mem_start = 0, phy_mem_start = 0, bit_offset = 0;

	ASSERT(!IS_KERNEL_THREAD(curr));

	if (0 != bitmap_scan(&(mem_pool->pool_bitmap), 1, &bit_offset))
		goto FAIL;

	phy_mem_start = mem_pool->phy_mem_offset + bit_offset * PG_SIZE;
	vir_mem_start = (U_HEAP_LIMIT + 1);

	bitmap_set(&(mem_pool->pool_bitmap), bit_offset, 1);
	page_table_add(vir_mem_start,  phy_mem_start);

	return vir_mem_start;

FAIL:
	return 0;
}

static uint32_t create_page_dir(void)
{
	uint32_t page_base_phy_addr = 0;
	thread_t *curr = running_thread();
	uint32_t *page_base_vaddr = NULL, *kernel_page_base_vaddr = (uint32_t*)0xfffff000;

	ASSERT(IS_KERNEL_THREAD(curr));

	page_base_vaddr = (uint32_t*) malloc_page(PF_KERNEL, 1);
	ASSERT(NULL != page_base_vaddr);
	page_base_vaddr[0x300] = kernel_page_base_vaddr[0x300];

	page_base_phy_addr = addr_v2p((uint32_t)page_base_vaddr);
	ASSERT(0 == (page_base_phy_addr & 0x00000fff));
	page_base_vaddr[1023] = page_base_phy_addr | PG_US_U | PG_RW_W | PG_P_1;

	return page_base_phy_addr;
}

static void start_process(void* function) {
	thread_t *curr = running_thread();
	intr_stack_t intr_stack;
	uint32_t proc_stack = malloc_proc_stack();

	ASSERT(!IS_KERNEL_THREAD(curr));
	ASSERT(0 != proc_stack);

	intr_stack.edi = intr_stack.esi = intr_stack.ebp = intr_stack.esp_dummy = 0;
	intr_stack.ebx = intr_stack.edx = intr_stack.ecx = intr_stack.eax = 0;
	intr_stack.gs = 0;
	intr_stack.fs = intr_stack.es = intr_stack.ds = SELECTOR_U_DATA;
	intr_stack.eip = function;
	intr_stack.cs = SELECTOR_U_CODE;
	intr_stack.eflags = (EFLAGS_IOPL_0 | EFLAGS_MBS | EFLAGS_IF_1);
	intr_stack.esp = (void*)(proc_stack + PG_SIZE);
	intr_stack.ss = SELECTOR_U_DATA;

	asm volatile (" \
		movl %0, %%esp;		\
		addl $4, %%esp;		\
		popal;		\
		popw %%gs;		\
		popw %%fs;		\
		popw %%es;		\
		popw %%ds;		\
		iretl;		\
		": : "g"(&intr_stack) : "memory");
}

void user_prog_ready(void *function, int8_t *name)
{
	thread_t *curr = running_thread();
	uint8_t priority = 1;

	ASSERT(IS_KERNEL_THREAD(curr));

	thread_t *thread = (thread_t*) malloc_page(PF_KERNEL, 1);
	thread_init(thread, name, priority);
	thread_func_setup(thread, start_process, function);
	thread->page_base_phy_addr = create_page_dir();
	thread_ready(thread);
}

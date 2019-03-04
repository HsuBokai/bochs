#include "tss.h"
#include "global.h"
#include "console.h"
#include "mm.h"
#include "bitmap.h"

tss_t tss;

void update_tss_esp(thread_t *self)
{
	tss.esp0 = (uint32_t*)((uint32_t)self + PG_SIZE);
}

void tss_init(void) {
	uint32_t tss_size = 0, tss_base = (uint32_t)&tss;
	uint8_t *tss_descriptor_base = (uint8_t*)(0xc0000903 + 4 * 8);

	console_put_str("tss_init start\n");
	tss_size = sizeof(tss);
	memset((uint8_t*)&tss, 0, tss_size);
	tss.ss0 = SELECTOR_K_STACK;
	tss.io_base = tss_size;

	tss_descriptor_base[0] = (uint8_t)((tss_size - 1) & 0x000000ff);
	tss_descriptor_base[1] = (uint8_t)(((tss_size - 1) & 0x0000ff00) >> 8);
	tss_descriptor_base[2] = (uint8_t)(tss_base & 0x000000ff);
	tss_descriptor_base[3] = (uint8_t)((tss_base & 0x0000ff00) >> 8);
	tss_descriptor_base[4] = (uint8_t)((tss_base & 0x00ff0000) >> 16);
	tss_descriptor_base[6] &= ~(0x0f);
	tss_descriptor_base[6] |= (uint8_t)(((tss_size - 1) & 0x000f0000) >> 16);
	tss_descriptor_base[7] = (uint8_t)((tss_base & 0xff0000000) >> 24);

	uint64_t gdt_operand = ((8*7 - 1) | ((uint64_t)(uint32_t)0xc0000903 << 16));

	asm volatile("lgdt %0" : : "m" (gdt_operand));
	asm volatile("ltr %w0" : : "r" (SELECTOR_TSS));

	console_put_str("tss_init done\n");
}

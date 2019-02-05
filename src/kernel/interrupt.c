#include "interrupt.h"
#include "global.h"
#include "io.h"
#include "print.h"
#include "debug.h"
#include "thread.h"
#include "schedule.h"

#define IDT_DESC_CNT 0x21

#define PIC_M_CTRL 0x20
#define PIC_M_DATA 0x21
#define PIC_S_CTRL 0xa0
#define PIC_S_DATA 0xa1

struct gate_desc {
	uint16_t func_offset_low_word;
	uint16_t selector;
	uint8_t dcount;
	uint8_t attribute;
	uint16_t func_offset_high_word;
};

static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function);
static struct gate_desc idt[IDT_DESC_CNT];
static int8_t* intr_name[IDT_DESC_CNT];

extern intr_handler intr_entry_table[IDT_DESC_CNT];

void general_intr_handler(uint8_t vec_nr)
{
	put_str("interrupt!! vec_nr:");
	put_int(vec_nr);
	put_str("!! intr_name:");
	put_str(intr_name[vec_nr]);
	put_str("\n");
	ASSERT(0);
}

void timer_intr_handler(uint8_t vec_nr)
{
	put_str("interrupt:");
	put_str(intr_name[vec_nr]);
	put_str("\n");

	thread_t *curr = running_thread();

	ASSERT(0x12345678 == curr->stack_magic);

	curr->elapsed_ticks++;

	if (0 == curr->ticks) {
		schedule();
	} else {
		curr->ticks--;
	}
}

static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function) {
	p_gdesc->func_offset_low_word = (uint32_t)function & 0x0000ffff;
	p_gdesc->selector = SELECTOR_K_CODE;
	p_gdesc->dcount = 0;
	p_gdesc->attribute = attr;
	p_gdesc->func_offset_high_word = ((uint32_t)function & 0xffff0000) >> 16;
}

static void idt_desc_init(void) {
	int i;
	for (i=0; i<IDT_DESC_CNT; i++) {
		make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0, intr_entry_table[i]);
	}
	intr_name[0x00] = "#DE Divide Error";
	intr_name[0x01] = "#DB Debug Exception";
	intr_name[0x02] = "#NMI Interrupt";
	intr_name[0x03] = "#BP Breakpoint Exception";
	intr_name[0x04] = "#OF Overflow Exception";
	intr_name[0x05] = "#BR Bound Range Exceeded Exception";
	intr_name[0x06] = "#UD Invalid Opcode Exception";
	intr_name[0x07] = "#NM Device Not Available Exception";
	intr_name[0x08] = "#DF Double Fault Exception";
	intr_name[0x09] = "#CSO Coprocessor Segment Overrun";
	intr_name[0x0a] = "#TS Invalid TSS Exception";
	intr_name[0x0b] = "#NP Segment Not Present";
	intr_name[0x0c] = "#SS Stack Fault Exception";
	intr_name[0x0d] = "#GP General Protection Exception";
	intr_name[0x0e] = "#PF Page-Fault Exception";
	intr_name[0x0f] = "#UU Unused";

	intr_name[0x10] = "#MF x87 FPU Floating-Point Error";
	intr_name[0x11] = "#AC Alignment Check Exception";
	intr_name[0x12] = "#MC Machine-Check Exception";
	intr_name[0x13] = "#XF SIMD Floating-Point Exception";
	intr_name[0x14] = "#UK Unknown";
	intr_name[0x15] = "#UK Unknown";
	intr_name[0x16] = "#UK Unknown";
	intr_name[0x17] = "#UK Unknown";
	intr_name[0x18] = "#UK Unknown";
	intr_name[0x19] = "#UK Unknown";
	intr_name[0x1a] = "#UK Unknown";
	intr_name[0x1b] = "#UK Unknown";
	intr_name[0x1c] = "#UK Unknown";
	intr_name[0x1d] = "#UK Unknown";
	intr_name[0x1e] = "#UK Unknown";
	intr_name[0x1f] = "#UK Unknown";

	intr_name[0x20] = "#TI Timer Interrupt";

	put_str("idt_desc_init done\n");
}

static void pic_init(void)
{
	outb(PIC_M_CTRL, 0x11);
	outb(PIC_M_DATA, 0x20);

	outb(PIC_M_DATA, 0x04);
	outb(PIC_M_DATA, 0x01);

	outb(PIC_S_CTRL, 0x11);
	outb(PIC_S_DATA, 0x28);

	outb(PIC_S_DATA, 0x02);
	outb(PIC_S_DATA, 0x01);

	outb(PIC_M_DATA, 0xfe);
	outb(PIC_S_DATA, 0xff);

	put_str("pic_init done\n");
}

void idt_init(void) {
	put_str("idt_init start\n");
	idt_desc_init();
	pic_init();

	uint64_t idt_operand = ((sizeof(idt) - 1) | ((uint64_t)(uint32_t)idt) << 16);
	asm volatile("lidt %0" : :  "m" (idt_operand));
	put_str("idt_init done\n");
}

uint32_t is_interrupt_enabled(void) {
	uint32_t ret = 0;
	GET_EFLAGS(ret);
	return (EFLAGS_IF & ret);
}

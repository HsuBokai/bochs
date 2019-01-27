#include "interrupt.h"
#include "stdint.h"
#include "global.h"

#define IDT_DESC_CNT 0x21

struct gate_desc {
	uint16_t func_offset_low_word;
	uint16_t selector;
	uint8_t dcount;
	uint8_t attribute;
	uint16_t func_offset_high_word;
};

static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function);
static struct gate_desc idt[IDT_DESC_CNT];

//#define MAKE_IDT_DESC(VEC_NR) make_idt_desc(&idt[ VEC_NR ], IDT_DESC_ATTR_DPL0, intr_##VEC_NR##_handler)
#define INTR_HANDLER(VEC_NR) intr_##VEC_NR##_handler

#define INTR_HANDLER_DECLARE(VEC_NR) \
	static void intr_##VEC_NR##_handler()   \
	{                                       \
		/* interrupt push eflags, push cs, push eip, (push error_code) */		\
		/* C funtion push ebp */							\
		asm volatile("pusha;");		\
		*((uint16_t*)(0xc00b8000 + 2*(VEC_NR + 640)))=(0x0700 + 0x61 + VEC_NR);		\
		asm volatile("popa;");		\
		/* for skip error_code */							\
		asm volatile("addl $4, %esp");	\
		/* C funtion pop ebp, return to cs:eip */					\
		/* uncorrectable bug in C function!!! */					\
		return;                         \
	}                                       \

INTR_HANDLER_DECLARE(0x00)
INTR_HANDLER_DECLARE(0x01)
INTR_HANDLER_DECLARE(0x02)
INTR_HANDLER_DECLARE(0x03)
INTR_HANDLER_DECLARE(0x04)
INTR_HANDLER_DECLARE(0x05)
INTR_HANDLER_DECLARE(0x06)
INTR_HANDLER_DECLARE(0x07)
INTR_HANDLER_DECLARE(0x08)
INTR_HANDLER_DECLARE(0x09)
INTR_HANDLER_DECLARE(0x0a)
INTR_HANDLER_DECLARE(0x0b)
INTR_HANDLER_DECLARE(0x0c)
INTR_HANDLER_DECLARE(0x0d)
INTR_HANDLER_DECLARE(0x0e)
INTR_HANDLER_DECLARE(0x0f)

INTR_HANDLER_DECLARE(0x10)
INTR_HANDLER_DECLARE(0x11)
INTR_HANDLER_DECLARE(0x12)
INTR_HANDLER_DECLARE(0x13)
INTR_HANDLER_DECLARE(0x14)
INTR_HANDLER_DECLARE(0x15)
INTR_HANDLER_DECLARE(0x16)
INTR_HANDLER_DECLARE(0x17)
INTR_HANDLER_DECLARE(0x18)
INTR_HANDLER_DECLARE(0x19)
INTR_HANDLER_DECLARE(0x1a)
INTR_HANDLER_DECLARE(0x1b)
INTR_HANDLER_DECLARE(0x1c)
INTR_HANDLER_DECLARE(0x1d)
INTR_HANDLER_DECLARE(0x1e)
INTR_HANDLER_DECLARE(0x1f)

INTR_HANDLER_DECLARE(0x20)

//extern intr_handler intr_entry_table[IDT_DESC_CNT];
static void intr_default_handler()
{
	asm volatile(" \
		pusha;			\
		pusha;			\
		pusha;			\
		pusha;			\
		popa;			\
		popa;			\
		popa;			\
		popa;			\
	");
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
	//for (i=0; i<IDT_DESC_CNT; i++) {
	//	make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0, intr_default_handler);
	//}

	make_idt_desc(&idt[0x00], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x00));
	make_idt_desc(&idt[0x01], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x01));
	make_idt_desc(&idt[0x02], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x02));
	make_idt_desc(&idt[0x03], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x03));
	make_idt_desc(&idt[0x04], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x04));
	make_idt_desc(&idt[0x05], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x05));
	make_idt_desc(&idt[0x06], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x06));
	make_idt_desc(&idt[0x07], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x07));
	make_idt_desc(&idt[0x08], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x08));
	make_idt_desc(&idt[0x09], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x09));
	make_idt_desc(&idt[0x0a], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x0a));
	make_idt_desc(&idt[0x0b], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x0b));
	make_idt_desc(&idt[0x0c], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x0c));
	make_idt_desc(&idt[0x0d], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x0d));
	make_idt_desc(&idt[0x0e], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x0e));
	make_idt_desc(&idt[0x0f], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x0f));

	make_idt_desc(&idt[0x10], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x10));
	make_idt_desc(&idt[0x11], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x11));
	make_idt_desc(&idt[0x12], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x12));
	make_idt_desc(&idt[0x13], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x13));
	make_idt_desc(&idt[0x14], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x14));
	make_idt_desc(&idt[0x15], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x15));
	make_idt_desc(&idt[0x16], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x16));
	make_idt_desc(&idt[0x17], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x17));
	make_idt_desc(&idt[0x18], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x18));
	make_idt_desc(&idt[0x19], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x19));
	make_idt_desc(&idt[0x1a], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x1a));
	make_idt_desc(&idt[0x1b], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x1b));
	make_idt_desc(&idt[0x1c], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x1c));
	make_idt_desc(&idt[0x1d], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x1d));
	make_idt_desc(&idt[0x1e], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x1e));
	make_idt_desc(&idt[0x1f], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x1f));

	make_idt_desc(&idt[0x20], IDT_DESC_ATTR_DPL0, INTR_HANDLER(0x20));

	put_str("idt_desc_init done\n");
}

void idt_init() {
	put_str("idt_init start\n");
	idt_desc_init();
	//pic_init();

	uint64_t idt_operand = ((sizeof(idt) - 1) | ((uint64_t)(uint32_t)idt) << 16);
	asm volatile("lidt %0" : :  "m" (idt_operand));
	put_str("idt_init done\n");
}

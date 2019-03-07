#ifndef __INTERRUPT_H
#define __INTERRUPT_H
#include "stdint.h"

typedef void* intr_handler;

#define IDT_DESC_CNT 0x30
#define GET_EFLAGS(EFLAGS_VAR) \
	asm volatile("pushfl; popl %0" : "=g" (EFLAGS_VAR))
#define INTR_DISABLE \
	asm volatile("cli" : : : "memory");
#define INTR_ENABLE \
	asm volatile("sti");

typedef enum pic_master_irq {
	PIC_M_IRQ_TIMER = 0,
	PIC_M_IRQ_KEYBOARD,
	PIC_M_IRQ_NONE,
	PIC_M_IRQ_SERIAL2,
	PIC_M_IRQ_SERIAL1,
	PIC_M_IRQ_PARALLAL1,
	PIC_M_IRQ_FLOPPY,
	PIC_M_IRQ_PARALLEL2
} pic_master_irq_t;

extern int8_t* intr_name[IDT_DESC_CNT];

void general_intr_handler(uint8_t vec_nr);
void timer_intr_handler(uint8_t vec_nr);
void idt_init(void);
uint32_t is_interrupt_enabled(void);

#endif /* __INTERRUPT_H */

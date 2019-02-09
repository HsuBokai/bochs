#ifndef __INTERRUPT_H
#define __INTERRUPT_H
#include "stdint.h"

typedef void* intr_handler;

#define IDT_DESC_CNT 0x30
#define EFLAGS_IF 0x00000200
#define GET_EFLAGS(EFLAGS_VAR) \
	asm volatile("pushfl; popl %0" : "=g" (EFLAGS_VAR))
#define INTR_DISABLE \
	asm volatile("cli" : : : "memory");
#define INTR_ENABLE \
	asm volatile("sti");

extern int8_t* intr_name[IDT_DESC_CNT];

void general_intr_handler(uint8_t vec_nr);
void timer_intr_handler(uint8_t vec_nr);
void idt_init(void);
uint32_t is_interrupt_enabled(void);

#endif /* __INTERRUPT_H */

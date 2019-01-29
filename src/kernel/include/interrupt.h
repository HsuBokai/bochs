#ifndef __INTERRUPT_H
#define __INTERRUPT_H

typedef void* intr_handler;

#define EFLAGS_IF 0x00000200
#define GET_EFLAGS(EFLAGS_VAR) \
	asm volatile("pushfl; popl %0" : "=g" (EFLAGS_VAR))
#define INTR_DISABLE \
	asm volatile("cli" : : : "memory");
#define INTR_ENABLE \
	asm volatile("sti");

#endif /* __INTERRUPT_H */

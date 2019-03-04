#ifndef __PROCESS_H
#define __PROCESS_H
#include "stdint.h"

typedef struct intr_stack {
	/* push %1; in intr.S */
	uint32_t vec_no;

	/* pushad; in intr.S */
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp_dummy;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;

	/* push gs; in intr.S */
	uint16_t gs;
	/* push fs; in intr.S */
	uint16_t fs;
	/* push es; in intr.S */
	uint16_t es;
	/* push ds; in intr.S */
	uint16_t ds;

	/* Timer Interrupt (vec_no: 0x20) with NO_ERROR_CODE */
	//uint32_t err_code;

	/* Auto Push Registers by CPU Handle Interrupts */
	void (*eip) (void);
	uint32_t cs;
	uint32_t eflags;
	void* esp;
	uint32_t ss;
} intr_stack_t;

void user_prog_ready(void *function, int8_t *name);

#endif /* __PROCESS_H */

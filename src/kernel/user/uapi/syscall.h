#ifndef __SYSCALL_H
#define __SYSCALL_H
#include "stdint.h"

typedef enum syscall_nr {
	SYS_GETPID,
	SYS_THREAD_YIELD,
	SYS_THREAD_BLOCK
} syscall_nr_t;

#define _syscall0(NUMBER) ({	\
	int retval;		\
	asm volatile (		\
		"int $0x80"	\
		: "=a" (retval)	\
		: "a" (NUMBER)	\
		: "memory"	\
	);			\
	retval;			\
})

uint32_t getpid(void);
uint32_t thread_yield(void);
uint32_t thread_block(void);

#endif /* __SYSCALL_H */

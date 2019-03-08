#ifndef __SYSCALL_H
#define __SYSCALL_H
#include "stdint.h"

typedef enum syscall_nr {
	SYS_GETPID
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

#endif /* __SYSCALL_H */

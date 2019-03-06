#ifndef __SYSCALL_INIT_H
#define __SYSCALL_INIT_H
#include "stdint.h"

#define SYSCALL_NR 32

typedef void* syscall;

uint32_t sys_getpid(void);
void syscall_init(void);

#endif /* __SYSCALL_INIT_H */

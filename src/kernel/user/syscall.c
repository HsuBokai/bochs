#include "syscall.h"

uint32_t getpid(void)
{
	return _syscall0(SYS_GETPID);
}

uint32_t thread_yield(void)
{
	return _syscall0(SYS_THREAD_YIELD);
}

uint32_t thread_block(void)
{
	return _syscall0(SYS_THREAD_BLOCK);
}

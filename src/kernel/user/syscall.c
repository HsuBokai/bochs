#include "syscall.h"

uint32_t getpid(void)
{
	return _syscall0(SYS_GETPID);
}

#include "syscall_init.h"
#include "syscall.h"
#include "console.h"
#include "thread.h"

syscall syscall_table[SYSCALL_NR];

uint32_t sys_getpid(void) {
	return running_thread()->pid;
}

void syscall_init(void) {
	console_put_str("syscall_init start\n");
	syscall_table[SYS_GETPID] = sys_getpid;
	console_put_str("syscall_init done\n");
}

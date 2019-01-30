#include "debug.h"
#include "interrupt.h"
#include "print.h"

void panic_spin(const char* filename, const int line, const char* func, const char* condition)
{
	if(is_interrupt_enabled()) INTR_DISABLE;

	put_str("\n\n\n!!!!! KERNEL PANIC !!!!!\n");
	put_str(filename);
	put_str(":");
	put_int(line);
	put_str(":");
	put_str(func);
	put_str(" ASSERT(");
	put_str(condition);
	put_str(")\n");

	while(1);
}

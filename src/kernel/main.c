#include "debug.h"
#include "interrupt.h"
#include "print.h"
#include "mm.h"

int in_a=1, in_b=2, out_sum;

int main(void){
	asm(" \
		pusha;			\
		movl in_a, %eax;	\
		movl in_b, %ebx;	\
		addl %ebx, %eax;	\
		movl %eax, out_sum;	\
		popa			\
	");
	put_str("hello world!\n");
	put_str("This is kernel!\n");

	mem_init();
	//malloc_page(PF_KERNEL, 10);
	//malloc_page(PF_KERNEL, 20);
	//malloc_page(PF_KERNEL, 30);
	//malloc_page(PF_KERNEL, 40);

	idt_init();
	asm volatile("sti");

	//ASSERT(1==2);

	while(1);
	return 0;
}

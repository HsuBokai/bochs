#include "stdint.h"

void put_str(uint8_t* str);

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

	idt_init();
	asm volatile("sti");

	while(1);
	return 0;
}

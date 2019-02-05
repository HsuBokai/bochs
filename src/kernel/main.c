#include "debug.h"
#include "interrupt.h"
#include "print.h"
#include "mm.h"
#include "thread.h"

int in_a=1, in_b=2, out_sum;

static void k_thread_a(void *arg);

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

	thread_t *thread_a = (thread_t*) malloc_page(PF_KERNEL, 1);
	thread_init(thread_a, "k_thread_a", 31);
	thread_func_setup(thread_a, k_thread_a, "thread A\n");
	thread_start(thread_a);

	while(1);
	return 0;
}

static void k_thread_a(void *arg)
{
	int8_t* str = arg;
	while(1) {
		put_str(str);
	}
}

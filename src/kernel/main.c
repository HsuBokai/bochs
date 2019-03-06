#include "debug.h"
#include "interrupt.h"
#include "console.h"
#include "mm.h"
#include "thread.h"
#include "timer.h"
#include "tss.h"
#include "process.h"

int in_a=1, in_b=2, out_sum;
uint8_t test_var_a = 0, test_var_b = 0;

static void print_thread(void *arg);
static void idle_thread(void *arg);
static void k_thread_a(void *arg);
static void k_thread_b(void *arg);
static void u_prog_a(void);
static void u_prog_b(void);

static void test_thread(void)
{
	thread_t *thread_a = (thread_t*) malloc_page(PF_KERNEL, 1);
	thread_init(thread_a, "thread A", 3);
	thread_func_setup(thread_a, print_thread, "thread A! ");
	thread_ready(thread_a);

	thread_t *thread_b = (thread_t*) malloc_page(PF_KERNEL, 1);
	thread_init(thread_b, "thread BB", 7);
	thread_func_setup(thread_b, print_thread, "thread BB! ");
	thread_ready(thread_b);

	thread_t *thread_main = (thread_t*) malloc_page(PF_KERNEL, 1);
	thread_init(thread_main, "thread Main", 1);
	thread_func_setup(thread_main, print_thread, "thread Main! ");
	thread_start(thread_main);
}

static void test_user_prog(void)
{
	thread_t *thread_a = (thread_t*) malloc_page(PF_KERNEL, 1);
	thread_init(thread_a, "thread A", 3);
	thread_func_setup(thread_a, k_thread_a, NULL);
	thread_ready(thread_a);

	thread_t *thread_b = (thread_t*) malloc_page(PF_KERNEL, 1);
	thread_init(thread_b, "thread B", 7);
	thread_func_setup(thread_b, k_thread_b, NULL);
	thread_ready(thread_b);

	user_prog_ready(u_prog_a, "user A");
	user_prog_ready(u_prog_b, "user B");
}

int main(void){
	asm(" \
		pusha;			\
		movl in_a, %eax;	\
		movl in_b, %ebx;	\
		addl %ebx, %eax;	\
		movl %eax, out_sum;	\
		popa			\
	");
	console_init();
	console_put_str("hello world!\n");
	console_put_str("This is kernel!\n");

	mem_init();
	//malloc_page(PF_KERNEL, 10);
	//malloc_page(PF_KERNEL, 20);
	//malloc_page(PF_KERNEL, 30);
	//malloc_page(PF_KERNEL, 40);

	idt_init();

	timer_init();

	tss_init();

	//ASSERT(1==2);

	//test_thread();
	test_user_prog();

	thread_t *thread_main = (thread_t*) malloc_page(PF_KERNEL, 1);
	thread_init(thread_main, "thread Main", 1);
	thread_func_setup(thread_main, idle_thread, NULL);
	thread_start(thread_main);

	return 0;
}

static void print_thread(void *arg)
{
	int8_t* str = arg;
	while(1) {
		console_put_str(str);
	}
}

static void idle_thread(void *arg)
{
	while(1);
}

static void k_thread_a(void *arg)
{
	while(1) {
		console_put_str("v_a:");
		console_put_int(test_var_a);
		console_put_str("\n");
	}
}

static void k_thread_b(void *arg)
{
	while(1) {
		console_put_str("v_b:");
		console_put_int(test_var_b);
		console_put_str("\n");
	}
}

static void u_prog_a(void)
{
	while(1) {
		test_var_a++;
	}
}

static void u_prog_b(void)
{
	while(1) {
		test_var_b++;
	}
}

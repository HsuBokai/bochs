#include "console.h"
#include "semaphore.h"
#include "print.h"

semaphore_t console_sema;

static void console_acquire(void)
{
	sema_down(&console_sema);
}

static void console_release(void)
{
	sema_up(&console_sema);
}

void console_init(void)
{
	put_str("console init start\n");
	sema_init(&console_sema);
	put_str("console init done\n");
}

void console_put_str(const int8_t* str)
{
	console_acquire();
	put_str(str);
	console_release();
}

void console_put_int(const uint8_t num)
{
	console_acquire();
	put_int(num);
	console_release();
}

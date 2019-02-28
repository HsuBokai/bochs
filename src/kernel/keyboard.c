#include "keyboard.h"
#include "print.h"
#include "io.h"
#include "interrupt.h"

#define KBD_BUF_PORT 0x60

void keyboard_intr_handler(uint8_t vec_nr)
{
#if 1
	put_str("interrupt:");
	put_str(intr_name[vec_nr]);
	put_str("\n");
#endif
	uint8_t scancode = inb(KBD_BUF_PORT);
	put_int(scancode);
}

//void keyboard_bottom_half(void *arg)
//{
//}

#ifndef __CONSOLE_H
#define __CONSOLE_H
#include "stdint.h"

void console_init(void);
void console_put_str(const int8_t* str);
void console_put_int(const uint8_t num);

#endif /* __CONSOLE_H */

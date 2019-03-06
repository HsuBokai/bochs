#ifndef __THREAD_PID_H
#define __THREAD_PID_H
#include "stdint.h"

typedef int16_t pid_t;

void thread_pid_init(void);
pid_t allocate_pid(void);

#endif /* __THREAD_PID_H */

#ifndef __SCHEDULE_H
#define __SCHEDULE_H
#include "list.h"

extern list_t thread_ready_list;
extern list_t thread_all_list;

void schedule(void);

#endif /* __SCHEDULE_H */

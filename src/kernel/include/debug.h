#ifndef __DEBUG_H
#define __DEBUG_H

void panic_spin(const char* filename, const int line, const char* func, const char* condition);

#define PANIC(...) panic_spin(__FILE__, __LINE__, __func__, __VA_ARGS__);

#ifdef NDEBUG
	#define ASSERT(CONDITION) ((void)0)
#else /* NDEBUG */
	#define ASSERT(CONDITION) \
		if(!(CONDITION)) PANIC(#CONDITION)
#endif /* NDEBUG */

#endif /* __DEBUG_H */

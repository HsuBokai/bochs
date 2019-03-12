#include "timer.h"
#include "print.h"
#include "io.h"
#include "syscall.h"

#define IRQ0_FREQUENCY 100 //Hz
#define INPUT_FREQUENCY 1193180
#define COUNTER0_VALUE (INPUT_FREQUENCY / IRQ0_FREQUENCY)
#define COUNTER0_PORT 0x40
#define COUNTER0_NO 0
#define COUNTER_MODE 2
#define READ_WRITE_LATCH 3
#define PIT_CONTROL_PORT 0x43

uint32_t kernel_ticks = 0;

static void frequency_set(uint8_t counter_port, uint8_t counter_no, uint8_t rwl, uint8_t counter_mode, uint16_t counter_value)
{
	outb(PIT_CONTROL_PORT, (uint8_t)(counter_no << 6 | rwl << 4 | counter_mode << 1));
	outb(counter_port, (uint8_t)counter_value);
	outb(counter_port, (uint8_t)counter_value >> 8);
}

void timer_init(void)
{
	put_str("timer init start\n");
	frequency_set(COUNTER0_PORT, COUNTER0_NO, READ_WRITE_LATCH, COUNTER_MODE, COUNTER0_VALUE);
	put_str("timer init done\n");
}

void msleep(uint32_t m_seconds)
{
	uint32_t start_tick = kernel_ticks;
	uint32_t sleep_ticks = m_seconds * IRQ0_FREQUENCY / 1000;
	while (kernel_ticks - start_tick < sleep_ticks) {
		thread_yield();
	}
}

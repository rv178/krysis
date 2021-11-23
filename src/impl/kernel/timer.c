#include "timer.h"
#include "port.h"
#include "print.h"
#include "isr.h"
#include "util.h"

uint32_t tick = 0;

static void timer_callback(registers_t *regs)
{
    tick++;
    print_str("Tick: ");

    char tick_ascii[256];
    int_to_string(tick, tick_ascii);
    print_str(tick_ascii);
    print_nl();
}

void init_timer(uint32_t freq)
{
    /** Install function */
    register_interrupt_handler(IRQ0, timer_callback);

    /** Get the PIT value: hwclock at 1193180hz */
    uint32_t divisor = 1193180 / freq;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);

    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
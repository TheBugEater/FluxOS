#include <kernel/pit.h>

void set_timer_speed(int hz)
{
    unsigned short divisor = 1193180 / hz;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
    pit_hz = hz;
}

void install_pit()
{
    set_timer_speed(100);
    add_interrupt_handler(32, timer_handler);
}

void timer_handler(struct cpu_state cpu, struct stack_state stack)
{
}




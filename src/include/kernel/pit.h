#ifndef __PIT_H__
#define __PIT_H__

#include <kernel/idt.h>

void install_pit();

void set_timer_speed(int hz);
void timer_handler(struct cpu_state cpu, struct stack_state stack);

static int pit_hz = 100;

#endif //__PIT_H__


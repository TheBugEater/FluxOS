#include <kernel/keyboard.h>

void install_keyboard()
{
    add_interrupt_handler(33, keyboard_handler);
}

void keyboard_handler(struct cpu_state cpu, struct stack_state stack)
{
    char scancode = inb(0x60);
    if(!(scancode & 0x80))
    {
        printk("%c", kbdus[scancode]);
    }
}

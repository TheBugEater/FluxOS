#include "terminal.h"
#include "idt.h"

void kmain()
{
    // Disable IRQ's
    disable_irq();

    install_idt();

    terminal_initialize();
    while(1);
}

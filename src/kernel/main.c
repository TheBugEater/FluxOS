#include "terminal.h"
#include "idt.h"
#include "utility.h"

void kmain()
{
    terminal_initialize();

    // Disable IRQ's
    disable_irq();

    install_idt();

    enable_irq();

    while(1);
}

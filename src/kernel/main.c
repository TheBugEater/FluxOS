#include "terminal.h"
#include "idt.h"
#include "gdt.h"
#include "utility.h"

void kmain()
{
    terminal_initialize();

    // Disable IRQ's
    disable_irq();

    install_gdt();
    install_idt();

    enable_irq();

    while(1);
}

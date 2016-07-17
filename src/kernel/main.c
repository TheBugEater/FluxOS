#include "terminal.h"
#include "idt.h"
#include "gdt.h"
#include "utility.h"
#include "keyboard.h"

void kmain()
{
    terminal_initialize();

    // Disable Interrupts
    disable_interrupts();

    install_gdt();
    install_idt();

    // Enable Interrupts
    enable_interrupts();

    install_keyboard();
  
    while(1);
}

#include "gdt.h"
#include "terminal.h"

void kmain(void)
{
    gdt_install();
    idt_install();

    __asm__ __volatile__ ("sti");

    terminal_initialize();

    // Trigger a division Zero interupt
    //
    int d = 10;
    printf("Hello World! %d", d);

    for(;;);
}

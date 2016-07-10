#include "gdt.h"
#include "terminal.h"

void kmain(void)
{
gdt_install();
idt_install();

__asm__ __volatile__ ("sti");

terminal_initialize();

int d = 10;
d = d/0;
printf("Hello World! %d", d);

for(;;);

}

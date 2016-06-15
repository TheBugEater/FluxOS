#include "gdt.h"
#include "terminal.h"

void kmain(void)
{
gdt_install();
idt_install();

terminal_initialize();

int d = 10;

printf("Hello World! %d", d);


while(1);

}

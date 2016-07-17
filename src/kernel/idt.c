#include "idt.h"
#include "utility.h"

void create_idt_entry(unsigned short num, unsigned int handler, unsigned short selector, unsigned char type) 
{
    idt[num].offset_low = handler & 0xFFFF;
    idt[num].offset_high = (handler >> 16) & 0xFFFF;
    idt[num].zero = 0;
    idt[num].selector = selector;
    idt[num].type_attr = type;
}

void install_idt()
{
    idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idtp.base = &idt;
    
    memset(&idt, 0, sizeof(struct idt_entry) * IDT_SIZE);

    load_idt((unsigned long*)&idtp);
}


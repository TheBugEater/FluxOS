#ifndef __IDT_H__
#define __IDT_H__

#define IDT_SIZE 256

extern void load_idt(unsigned long*);

struct idt_entry
{
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short offset_high;
}__attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
}__attribute__((packed));

struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

// Functions

void create_idt_entry(unsigned short num, unsigned int handler, unsigned short selector, unsigned char type);

void install_idt();

#endif

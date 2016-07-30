#ifndef __IDT_H__
#define __IDT_H__

#define IDT_SIZE 256
#define KERNEL_CODE_SEGMENT 0x08
#define IDT_GATE 0x8E

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

struct cpu_state
{
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
}__attribute__((packed));

struct stack_state
{
    unsigned int interrupt_num, error_code;
    unsigned int eip, cs, eflags;
}__attribute__((packed));


struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

typedef void (*interrupt_handler_fn)(struct cpu_state cpu, struct stack_state stack);
interrupt_handler_fn interrupt_handlers[IDT_SIZE];

// ISR Handlers
extern void isr_0();
extern void isr_1();
extern void isr_2();
extern void isr_3();
extern void isr_4();
extern void isr_5();
extern void isr_6();
extern void isr_7();
extern void isr_8();
extern void isr_9();
extern void isr_10();
extern void isr_11();
extern void isr_12();
extern void isr_13();
extern void isr_14();
extern void isr_15();
extern void isr_16();
extern void isr_17();
extern void isr_18();
extern void isr_19();
extern void isr_20();
extern void isr_21();
extern void isr_22();
extern void isr_23();
extern void isr_24();
extern void isr_25();
extern void isr_26();
extern void isr_27();
extern void isr_28();
extern void isr_29();
extern void isr_30();
extern void isr_31();

extern void irq_0();
extern void irq_1();
extern void irq_2();
extern void irq_3();
extern void irq_4();
extern void irq_5();
extern void irq_6();
extern void irq_7();
extern void irq_8();
extern void irq_9();
extern void irq_10();
extern void irq_11();
extern void irq_12();
extern void irq_13();
extern void irq_14();
extern void irq_15();

// Functions

void create_idt_entry(unsigned short num, unsigned long handler, unsigned short selector, unsigned char type);

void install_idt();

void irq_remap();
void install_isr();
void install_irq();

void kernel_idt_handler(struct cpu_state, struct stack_state);

void add_interrupt_handler(unsigned int interrupt_num, interrupt_handler_fn* function);

#endif

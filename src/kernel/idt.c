#include "idt.h"
#include "utility.h"

void create_idt_entry(unsigned short num, unsigned long handler, unsigned short selector, unsigned char type) 
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

    install_isr();
}

void install_isr()
{
    create_idt_entry(0, isr_0, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(1, isr_1, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(2, isr_2, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(3, isr_3, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(4, isr_4, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(5, isr_5, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(6, isr_6, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(7, isr_7, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(8, isr_8, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(9, isr_9, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(10, isr_10, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(11, isr_11, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(12, isr_12, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(13, isr_13, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(14, isr_14, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(15, isr_15, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(16, isr_16, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(17, isr_17, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(18, isr_18, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(19, isr_19, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(20, isr_20, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(21, isr_21, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(22, isr_22, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(23, isr_23, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(24, isr_24, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(25, isr_25, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(26, isr_26, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(27, isr_27, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(28, isr_28, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(29, isr_29, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(30, isr_30, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(31, isr_31, KERNEL_CODE_SEGMENT, IDT_GATE);
}

void kernel_idt_handler(unsigned int num, unsigned int error)
{
    printk("Interrupt : %d Occurred!", num);
    if(num < 32)
    {
        while(1);
    }
}

void install_irq()
{
}



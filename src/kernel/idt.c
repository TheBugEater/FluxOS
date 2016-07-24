#include <kernel/idt.h>
#include <utility/utility.h>

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
    printk("Installing IDT...\n");

    idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idtp.base = &idt;
    
    memset(&idt, 0, sizeof(struct idt_entry) * IDT_SIZE);
    memset(&interrupt_handlers, 0, sizeof(interrupt_handler_fn) * IDT_SIZE);

    load_idt((unsigned long*)&idtp);

    install_isr();
    install_irq();

    interrupt_handlers[81] = assert_interrupt;

    printk("IDT Installed Successfully...\n");
}

void irq_remap()
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0);
    outb(0xA1, 0);
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

void install_irq()
{
    irq_remap();

    create_idt_entry(32, irq_0, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(33, irq_1, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(34, irq_2, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(35, irq_3, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(36, irq_4, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(37, irq_5, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(38, irq_6, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(39, irq_7, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(40, irq_8, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(41, irq_9, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(42, irq_10, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(43, irq_11, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(44, irq_12, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(45, irq_13, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(46, irq_14, KERNEL_CODE_SEGMENT, IDT_GATE);
    create_idt_entry(47, irq_15, KERNEL_CODE_SEGMENT, IDT_GATE);
}

void kernel_idt_handler(struct cpu_state cpu, struct stack_state stack)
{
    kernel_assert(stack.interrupt_num < IDT_SIZE);

    // If there is a seperate handle for the exception don't handle it here
    if(stack.interrupt_num < 32 && !(interrupt_handlers[stack.interrupt_num]))
    {
        printk("Exception : %d Occurred! ErrorCode:%d\n", stack.interrupt_num, stack.error_code);
        for(;;);
    }

    if(stack.interrupt_num > 40)
    {
        outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);
    
    if(interrupt_handlers[stack.interrupt_num])
    {
        interrupt_handlers[stack.interrupt_num](cpu, stack);
    }
}

void add_interrupt_handler(unsigned int interrupt_num, interrupt_handler_fn* function)
{
    kernel_assert(interrupt_num < IDT_SIZE);

    interrupt_handlers[interrupt_num] = function;
    printk("Added Interrupt Handler for %d\n",interrupt_num);
}

void assert_interrupt(struct cpu_state cpu, struct stack_state stack)
{
    printk("Asserted Kernel\n");
}

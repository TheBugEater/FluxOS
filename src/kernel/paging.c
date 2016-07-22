#include <paging.h>
#include <mm.h>

void install_paging(kernel_boot_info_t* info)
{
    add_interrupt_handler(14, page_fault_handler);
    // Page Directory and Page Table Takes 4mb for Full Address Space
    // Reserve That
    // Gives us 4096 bytes, which is enough to hold 1024 page tables
    page_directory = (unsigned long*)new_block();
    unsigned long* page_table = (unsigned long*)new_block();
    for(unsigned int i = 0; i < 1024; i++)
    {
        // Set the Address and Make it Present and Writable
        page_table[i] = (i * 0x1000) | 3;
    }

    page_directory[0] = ((unsigned long)page_table & 0xFFFFF000) | 3;

    for(unsigned int i = 1; i < 1024; i++)
    {
        // Make Other addressess Blank
        page_directory[i] = 2;
    }
 
    unsigned long cr0 = get_cr0();
    unsigned long cr3 = get_cr3();

    cr0 = cr0 | (1 << 31);
    cr3 = cr3 | (unsigned long)page_directory;

    write_cr3(cr3);
    write_cr0(cr0);

    void* p = 0xA0000000;
    int a = *(int*)p;
}

void page_fault_handler(struct cpu_state cpu, struct stack_state stack)
{
    printk("Page Fault Occurred! %x",get_cr2());
    for(;;);
}

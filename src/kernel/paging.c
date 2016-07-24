#include <kernel/paging.h>
#include <kernel/mm.h>

void install_paging(kernel_boot_info_t* info)
{
    add_interrupt_handler(14, page_fault_handler);
    
    // Page Directory and Page Table Takes 4mb for Full Address Space
    // Reserve That
    // Gives us 4096 bytes, which is enough to hold 1024 page tables
    page_directory = (unsigned long*)new_block();
    unsigned long* page_table = (unsigned long*)new_block();

    unsigned long* v_page_directory = ADDR_TO_KERNEL_BASE(page_directory);
    unsigned long* v_page_table = ADDR_TO_KERNEL_BASE(page_table);

    printk("%x | %x\n",page_directory,page_table);
    for(unsigned int i = 0; i < 1024; i++)
    {
        // Set the Address and Make it Present and Writable
        v_page_table[i] = (i * 0x1000) | 3;
    }

    for(unsigned int i = 0; i < 1024; i++)
    {
        v_page_directory[i] = 2;
    }

    v_page_directory[768] = ((unsigned long)page_table & 0xFFFFF000) | 3;
    printk("%x\n",v_page_directory[768]);
 
    switch_page_directory(page_directory);

    printk("##########################\n");
    printk("### TESTING ALLOCATION ###\n");
    printk("##########################\n");
    
    int* a = (int*)0xAE50FFFF;
    *a = 10;
    /*
    for(int i=0; i<7000;i++)
    {
        void* p = (unsigned long)new_block() + KERNEL_VIRTUAL_BASE;
        int a = *(int*)p;
        a = 10;
        printk("A = i x 10 = %d\n",a*i);
    }
*/ 
}

BOOL add_page_for_virtual(unsigned long* virtual_addr, unsigned long attribs)
{
    unsigned long dir_index = VIRTUAL_ADDR_DIR(virtual_addr);
    unsigned long page_index = VIRTUAL_ADDR_PAGE(virtual_addr);
    unsigned long offset = VIRTUAL_ADDR_OFFSET(virtual_addr);
    unsigned long *new_directory = 0;
    unsigned long* v_page_directory = ADDR_TO_KERNEL_BASE(page_directory);

    return TRUE;
}


void page_fault_handler(struct cpu_state cpu, struct stack_state stack)
{
    
    page_fault_error_t* fault = (page_fault_error_t*)&stack.error_code;
    if(!fault->page_present)
    {
        printk("Page doesn't exist\n");
    }

    void* fault_addr = get_page_fault_addr();
    printk("Page Fault Occurred! %x | Error Code %d\n",get_page_fault_addr(), stack.error_code);
  
    for(;;);
}

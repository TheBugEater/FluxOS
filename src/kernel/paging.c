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
        if(i == 768)
        {
            v_page_directory[768] = ((unsigned long)page_table & 0xFFFFF000) | 3;
            printk("%x\n",v_page_directory[768]);
        }
        else
        {
            v_page_directory[i] = 2;
        }
    }

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
int var = 0;

BOOL add_page_for_virtual(unsigned long* virtual_addr, unsigned long attribs)
{
    unsigned long dir_index = VIRTUAL_ADDR_DIR(virtual_addr);
    unsigned long page_index = VIRTUAL_ADDR_PAGE(virtual_addr);
    unsigned long offset = VIRTUAL_ADDR_OFFSET(virtual_addr);
    unsigned long *new_directory = 0;
    unsigned long* v_page_directory = ADDR_TO_KERNEL_BASE(page_directory);

    printk("Virtual Address: %x | Dir Index: %d | Page Index: %d | Offset: %d\n",virtual_addr, dir_index, page_index, offset);

    if((*(pde_t*)&v_page_directory[dir_index]).present)
        printk("Directory Already Present : %x\n",v_page_directory[dir_index]);
    else
        printk("Directory Is Empty: %x\n",v_page_directory[dir_index]);


    pte_t p_table = *(pte_t*)&attribs;
    pde_t p_directory;
    memset(&p_directory,0,sizeof(pde_t));

    new_directory = (unsigned long*)new_block();
    unsigned long* v_new_directory = ADDR_TO_KERNEL_BASE(new_directory);

    unsigned long* new_page = (unsigned long*)new_block();
    p_table.page_base_addr = (unsigned long)new_page & 0xFFFFF000;
    p_table.present = TRUE;

    for(int i=0; i<1024; i++)
    {
        v_new_directory[i] = 2;
    }
    v_new_directory[page_index] = *(unsigned long*)&p_table;
    printk("P Table :%x\n",v_new_directory[page_index]);

    p_directory.present = TRUE;
    p_directory.writable = TRUE;
    p_directory.page_table_base_addr = (unsigned long)new_directory & 0xFFFFF000;

    v_page_directory[dir_index] = *(unsigned long*)&p_directory;
    printk("P DIRECTORY :%x\n",v_page_directory[dir_index]);

    invalidate_addr(virtual_addr);
    if(var)
        for(;;);

    var++;
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

  
    if(!add_page_for_virtual(fault_addr, PTE_WRITABLE))
    {
    }

}

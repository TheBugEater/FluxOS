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
        unsigned long* temp_page_table = (unsigned long*)new_block();
        pde_t p_dir = create_new_page_dir(0, temp_page_table);
        v_page_directory[i] = pde_t;
    }

    v_page_directory[768] = ((unsigned long)page_table & 0xFFFFF000) | 3;
    printk("%x\n",v_page_directory[768]);
 
    switch_page_directory(page_directory);

    virtual_heap_top = KERNEL_HEAP_BASE;

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

pte_t create_new_pte_entry(BOOL usr_mode, void* phys_addr)
{
    pte_t page_entry;
    page_entry.present = TRUE;
    page_entry.writable = TRUE;
    page_entry.user_mode = usr_mode;
    page_entry.page_base_addr = (unsigned long)phys_addr & 0xFFFFF000;
    return page_entry;
}

pde_t create_new_pde_entry(BOOL usr_mode, void* phys_addr)
{
    pde_t dir_entry;
    dir_entry.present = TRUE;
    dir_entry.writable = TRUE;
    dir_entry.user_mode = usr_mode;
    dir_entry.page_table_base_addr = (unsigned long)phys_addr & 0xFFFFF000;
    return dir_entry;
}

void* get_new_page()
{
    unsigned long* v_page_directory = ADDR_TO_KERNEL_BASE(page_directory);

    // Convert it to a Virtual Block of Kernel Heap
    unsigned long dir_index = VIRTUAL_ADDR_DIR(virtual_heap_top);
    unsigned long page_index = VIRTUAL_ADDR_PAGE(virtual_heap_top);

    pde_t page_dir = *(pde_t*)&v_page_directory[dir_index];
    if(!page_dir.present)
    {
        // Page Dir not set, Create One
        void* phys_dir_block = new_block();
        page_dir = create_new_pde_entry(0, phys_dir_block);
    }
   
    // Get A Physical Block
    void* phys_block = new_block();

    create_new_pte_entry(0, phys_block);

    // Increase offset
    virtual_heap_top = (unsigned long)virtual_heap_top + PAGE_SIZE;
}

void delete_page(void* ptr)
{
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

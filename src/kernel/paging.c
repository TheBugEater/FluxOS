#include <kernel/paging.h>
#include <kernel/mm.h>

void install_paging(kernel_boot_info_t* info)
{
    add_interrupt_handler(14, page_fault_handler);
    /****************************************************
     ***********************************|PDE ENTRIES|PDE|
     ****************************************************
     We will Map Page Directory and Page Tables to 
     Virtual Space
     * **************************************************/
    page_directory_phys_addr = (unsigned long*)new_block(); unsigned long* kernel_page_table = (unsigned long*)new_block();

    unsigned long* v_page_directory = ADDR_TO_KERNEL_BASE(page_directory_phys_addr);
    unsigned long* v_kernel_page_table = ADDR_TO_KERNEL_BASE(kernel_page_table);

    for(unsigned int i = 0; i < 1024; i++)
    {
        // Set the Address and Make it Present and Writable
        v_kernel_page_table[i] = (i * 0x1000) | 3;
    }

    for(unsigned int i = 1; i < 1024; i++)
    {
        v_page_directory[i] = 2;
    }
    // Map Kernel
    v_page_directory[768] = ((unsigned long)kernel_page_table & 0xFFFFF000) | 3;
    // Recusrive Page Table
    v_page_directory[1023] = ((unsigned long)page_directory_phys_addr & 0xFFFFF000) | 3; 

    switch_page_directory(page_directory_phys_addr);

    virtual_heap_top = KERNEL_HEAP_BASE;

    printk("##########################\n");
    printk("### TESTING ALLOCATION ###\n");
    printk("##########################\n");
    
    void* p = new_block();
    int* a = 0xAFF02220;
    int* b = 0xAFF02224;
    add_page_mapping(p, a);
    *a = 10;
    *b = 15;
    printk("Value is %d | B = %d\n",*a, *b);
}

pte_t create_new_pte_entry(BOOL usr_mode, void* phys_addr)
{
    pte_t page_entry;
    page_entry.present = TRUE;
    page_entry.writable = TRUE;
    page_entry.user_mode = usr_mode;
    page_entry.page_base_addr = ((unsigned long)phys_addr >> 20) & 0xFFFFF000;
    return page_entry;
}

pde_t create_new_pde_entry(BOOL usr_mode, void* phys_addr)
{
    pde_t dir_entry;
    dir_entry.present = TRUE;
    dir_entry.writable = TRUE;
    dir_entry.user_mode = usr_mode;
    dir_entry.page_table_base_addr = ((unsigned long)phys_addr >> 20) & 0xFFFFF000;
    return dir_entry;
}

void* create_virtual_addr(unsigned long page_table, unsigned long page_index, unsigned long offset)
{
    unsigned long addr = 0;
    addr |= (page_table << 22 & 0xFFFFF000);
    addr |= (page_index << 12 & 0x3FF000);
    addr |= (offset & 0x3FF);
    return (unsigned long*)addr;
}

void* add_page_mapping(unsigned long* physical_addr, unsigned long* virtual_addr)
{
    unsigned long table_index = GET_PAGE_TABLE_INDEX(virtual_addr);
    unsigned long page_index = GET_PAGE_INDEX(virtual_addr);

    // A Page is always at a Page Boundary, So the offset can always be 0
    void* modified_virt_addr = create_virtual_addr(table_index, page_index, 0);

    pde_t page_table = *(pde_t*)&page_directory[table_index];
    if(page_table.present)
    {
        unsigned long* virt_table = (unsigned long*)(0xFFC00000 + table_index * 0x1000);
        pte_t page = *(pte_t*)&virt_table[page_index];
        if(!page.present)
        {
            virt_table[page_index] = (unsigned long)physical_addr | 3;
        }
    }
    else
    {
        void* new_table = new_block();
        unsigned long* virt_table = (unsigned long*)(0xFFC00000 + table_index * 0x1000);

        page_directory[table_index] = (unsigned long)new_table | 3;
        virt_table[page_index] = (unsigned long)physical_addr | 3;
    }

    return modified_virt_addr;
}

void remove_page_mapping(unsigned long* virtual_addr)
{
    unsigned long table_index = GET_PAGE_TABLE_INDEX(virtual_addr);
    unsigned long page_index = GET_PAGE_INDEX(virtual_addr);

    pde_t page_table = *(pde_t*)&page_directory[table_index];
    if(page_table.present)
    {
        unsigned long* virt_table = (unsigned long*)(0xFFC00000 + table_index * 0x1000);
        pte_t page = *(pte_t*)&virt_table[page_index];
        if(page.present)
        {
            page.present = FALSE;
            page.page_base_addr &= 0;
            virt_table[page_index] = *(unsigned long*)&page;
            invalidate_addr(virtual_addr);
        }
    }
 }

void* get_new_page()
{
    // Convert it to a Virtual Block of Kernel Heap
    unsigned long page_table = GET_PAGE_TABLE_INDEX(virtual_heap_top);
    unsigned long page_index = GET_PAGE_INDEX(virtual_heap_top);

    pde_t page_dir = *(pde_t*)&page_directory[page_table];
    if(!page_dir.present)
    {
        // Page Dir not set, Create One
        void* phys_dir_block = new_block();
        page_dir = create_new_pde_entry(0, phys_dir_block);
        page_directory[page_table] = *(unsigned long*)&page_dir;
    }
   
    // Get A Physical Block
    void* phys_block = new_block();
    //void* new_page_addr = add_page_mapping(phys_block, virtual_heap_top);

    // Increase offset
    virtual_heap_top = (unsigned long)virtual_heap_top + PAGE_SIZE;

    return 0;//new_page_addr;
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

    unsigned long table_dir = GET_PAGE_TABLE_INDEX(fault_addr);
    unsigned long page_index = GET_PAGE_INDEX(fault_addr);
    printk("Page Directory: %d | Page Table:%d | Address in Page Table:%x \n", table_dir, page_index, page_directory[page_index]);
    for(;;);
}

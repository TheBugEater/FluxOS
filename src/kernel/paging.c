#include <paging.h>
#include <utility.h>

void create_page_table_entry(pte_entry* entry, unsigned long attribs, unsigned long frame)
{
    *entry = attribs | (frame >> 12 & PTE_FRAME);
}

void create_page_directory_entry(pde_entry* entry, unsigned long attribs, unsigned long frame)
{
    *entry = attribs | (frame >> 12 & PDE_FRAME);
}

void install_paging(unsigned long end)
{


    unsigned long cr0 = get_cr0();
    unsigned long cr3 = get_cr3();

    cr0 = cr0 | (1 << 31);
    cr3 = cr3 | (0xFFFFF000 & page_directory[0]);

    write_cr3(cr3);
    write_cr0(cr0);
}



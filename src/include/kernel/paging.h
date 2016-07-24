#ifndef __PAGING_H__
#define __PAGING_H__

#include <utility/utility.h>
#include <kernel/idt.h>

#define PTE_PRESENT         1 << 0
#define PTE_WRITABLE        1 << 1
#define PTE_USER_MODE       1 << 2
#define PTE_ACCESSED        1 << 5
#define PTE_DIRTY           1 << 6
#define PTE_FRAME           0xFFFFF000
#define PTE_KERNEL_WRITABLE PTE_PRESENT | PTE_WRITABLE

#define PDE_PRESENT         1 << 0
#define PDE_WRITABLE        1 << 1
#define PDE_USER_MODE       1 << 2
#define PDE_WRITE_THROUGH   1 << 3
#define PDE_CACHE_ENABLED   1 << 4
#define PDE_ACCESSED        1 << 5
#define PDE_4MB_PAGE        1 << 7
#define PDE_GLOBAL_PAGE     1 << 8
#define PDE_FRAME           0xFFFFF000
#define PDE_KERNEL_WRITABLE PDE_PRESENT | PDE_WRITABLE

#define PAGE_SIZE           4096

/*
 * Page directory entry datatype.
 * If marked as present, it specifies the physical address
 * and permissions of a page table.
 */
typedef struct {
    unsigned long present:1;
    unsigned long writable:1;
    unsigned long user_mode:1;
    unsigned long write_through:1;
    unsigned long cache_disabled:1;
    unsigned long accesed:1;
    unsigned long reserved:1;
    unsigned long large_pages:1;
    unsigned long global_page:1;
    unsigned long kernel_info:3;
    unsigned long page_table_base_addr:20;
}pde_t;

/*
 * Page table entry datatype.
 * If marked as present, it specifies the physical address
 * and permissions of a page of memory.
 */
typedef struct {
    unsigned long present:1;
    unsigned long flags:4;
    unsigned long accesed:1;
    unsigned long dirty:1;
    unsigned long pte_attribute:1;
    unsigned long global_page:1;
    unsigned long kernel_info:3;
    unsigned long page_base_addr:20;
}pte_t;

typedef struct 
{
    unsigned long page_present:1;
    unsigned long read_write:1;
    unsigned long user_mode:1;
    unsigned long reserved_overwrite:1;
    unsigned long instruction_fetch:1;
    unsigned long unused:27;
}__attribute__((packked)) page_fault_error_t;

static unsigned long* page_directory = (unsigned long *)0;
static unsigned long* kernel_page_table = (unsigned long *)0;

void install_paging(kernel_boot_info_t* info);

BOOL add_page_for_virtual(unsigned long* virtual_addr, unsigned long attribs);
void page_fault_handler(struct cpu_state cpu, struct stack_state stack);

#endif //__PAGING_H__

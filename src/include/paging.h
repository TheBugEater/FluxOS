#ifndef __PAGING_H__
#define __PAGING_H__

#include <utility.h>
#include <idt.h>

/*
 * Paging Break Down
 *
 * PTE(Page Table Entry) Structure
 * PDE(Page Directory Entry) Structure
 * PDT

########## Page Table Entry ################################
    Bit 0 (P): Present flag
        0: Page is not in memory
        1: Page is present (in memory)
    Bit 1 (R/W): Read/Write flag
        0: Page is read only
        1: Page is writable
    Bit 2 (U/S):User mode/Supervisor mode flag
        0: Page is kernel (supervisor) mode
        1: Page is user mode. Cannot read or write supervisor pages
    Bits 3-4 (RSVD): Reserved by Intel
    Bit 5 (A): Access flag. Set by processor
        0: Page has not been accessed
        1: Page has been accessed
    Bit 6 (D): Dirty flag. Set by processor
        0: Page has not been written to
        1: Page has been written to
    Bits 7-8 (RSVD): Reserved
    Bits 9-11 (AVAIL): Available for use
     Bits 12-31 (FRAME): Frame address

########## Page Directory Entry ################################
    Bit 0 (P): Present flag
        0: Page is not in memory
        1: Page is present (in memory)
    Bit 1 (R/W): Read/Write flag
        0: Page is read only
        1: Page is writable
    Bit 2 (U/S):User mode/Supervisor mode flag
        0: Page is kernel (supervisor) mode
        1: Page is user mode. Cannot read or write supervisor pages
    Bit 3 (PWT):Write-through flag
        0: Write back caching is enabled
        1: Write through caching is enabled
    Bit 4 (PCD):Cache disabled
        0: Page table will not be cached
        1: Page table will be cached
    Bit 5 (A): Access flag. Set by processor
        0: Page has not been accessed
        1: Page has been accessed
    Bit 6 (D): Reserved by Intel
    Bit 7 (PS): Page Size
        0: 4 KB pages
        1: 4 MB pages
    Bit 8 (G): Global Page (Ignored)
    Bits 9-11 (AVAIL): Available for use
    Bits 12-31 (FRAME): Page Table Base address
 * */

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
    unsigned long flags:4;
    unsigned long accesed:1;
    unsigned long reserved:1;
    unsigned long large_pages:1;
    unsigned long global_page:1;
    unsigned long kernel_info:3;
    unsigned long page_table_base_addr:20;
}__attribute((packed)) pde_t;

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
}__attribute((packed)) pte_t;

// Make the Page Directory and Page Table Point to 0
static unsigned long *page_directory = (unsigned long *) 0x0;

void install_paging(kernel_boot_info_t* info);
void page_fault_handler(struct cpu_state cpu, struct stack_state stack);

#endif //__PAGING_H__

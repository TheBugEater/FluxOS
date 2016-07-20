#ifndef __PAGING_H__
#define __PAGING_H__

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

typedef unsigned long pte_entry;
typedef unsigned long pde_entry;

static unsigned long *page_directory = (unsigned long *) 0x9C000;
static unsigned long *page_tables = (unsigned long *) 0x9D000;

void create_page_table_entry(pte_entry* entry, unsigned long attribs, unsigned long frame);
void create_page_directory_entry(pde_entry* entry, unsigned long attribs, unsigned long frame);

void install_paging(unsigned long end);

#endif //__PAGING_H__

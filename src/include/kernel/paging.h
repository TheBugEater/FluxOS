#ifndef __PAGING_H__
#define __PAGING_H__

#include <utility/utility.h>
#include <kernel/idt.h>

// Kernel Heap Starts from 128mb from Base
#define KERNEL_HEAP_BASE    KERNEL_VIRTUAL_BASE + (128 << 20)
#define KERNEL_HEAP_SIZE    (256 << 20)
#define PAGE_SIZE           4096
#define PAGE_ADDR_MASK      0xFFFFF000

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
    unsigned long writable:1;
    unsigned long user_mode:1;
    unsigned long reserved:2;
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

static unsigned long* page_tables_start = 0xFFC00000;
static unsigned long* page_directory = 0xFFFFF000;
static unsigned long* page_directory_phys_addr = 0;

void install_paging(kernel_boot_info_t* info);

pte_t create_new_pte_entry(BOOL usr_mode, void* phys_addr);
pde_t create_new_pde_entry(BOOL usr_mode, void* phys_addr);

void page_fault_handler(struct cpu_state cpu, struct stack_state stack);

void* create_virtual_addr(unsigned long page_table, unsigned long page_index, unsigned long offset);
void* add_page_mapping(unsigned long* physical_addr, unsigned long* virtual_addr);
void remove_page_mapping(unsigned long* virtual_addr);

void* get_pages(unsigned long blocks);
void free_pages(void* ptr, unsigned long blocks);

// Returns a New Page from Kernel Heap
void* new_page();

// Removes a Page 
void delete_page(void* ptr);

static void* virtual_heap_top = 0;

#endif //__PAGING_H__

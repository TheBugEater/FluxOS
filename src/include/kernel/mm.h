#ifndef __MM_H__
#define __MM_H__

#include <utility/utility.h>

static unsigned long* phys_alloc_start= 0;
static unsigned long num_blocks = 0;

static unsigned long* mem_map = 0;
static unsigned long mem_map_size = 0;

void install_mm(kernel_boot_info_t* info);

unsigned long round_next_page(unsigned long address);

unsigned long round_previous_page(unsigned long address);

// Returns Linear Blocks
void* get_blocks(unsigned long blocks);
void free_blocks(void* ptr, unsigned long blocks);

// Returns a New Block
void* new_block();
void delete_block(void* ptr);

#endif // __MM_H__

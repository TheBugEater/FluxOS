#ifndef __ALLOC_H__
#define __ALLOC_H__

#define UNALLOCATED 0xCCCC
#define ALLOCATED   0xFEDC

typedef struct
{
    struct mem_block_info* next;
    struct mem_block_info* merge_prev;
    struct mem_block_info* merge_next;
    unsigned long size;
    unsigned short available;
    unsigned short magic;
}mem_block_info;

static mem_block_info* block_head = 0;

void* kmalloc(unsigned long size);
void kfree(void* ptr);

void list_add(mem_block_info* ptr);
mem_block_info* get_mem_block(void* ptr);
mem_block_info* find_free_block(unsigned long size);
mem_block_info* request_memory(unsigned long size);

void split_mem(mem_block_info* base, unsigned long remaining_size);
mem_block_info* merge_mem_up(mem_block_info* current);
mem_block_info* merge_mem_down(mem_block_info* current);

#define MEM_BLOCK_SIZE sizeof(mem_block_info)

#endif //__ALLOC_H__

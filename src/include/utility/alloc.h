#ifndef __ALLOC_H__
#define __ALLOC_H__

#define UNALLOCATED 0xCCCCCCCC
#define ALLOCATED   0xABCDEF12

typedef struct
{
    struct mem_block_info* next;
    unsigned long size;
    unsigned long available;
    unsigned long magic;
} mem_block_info;

static mem_block_info* block_head = 0;

void* kmalloc(unsigned long size);
void kfree(void* ptr);

void list_add(mem_block_info* ptr);
mem_block_info* get_mem_block(void* ptr);
mem_block_info* find_free_block(unsigned long size);
mem_block_info* request_memory(unsigned long size);


#define MEM_BLOCK_SIZE sizeof(mem_block_info)

#endif //__ALLOC_H__

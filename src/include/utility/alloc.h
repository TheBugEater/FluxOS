#ifndef __ALLOC_H__
#define __ALLOC_H__

typedef struct
{
    struct mem_block_info* next;
    unsigned long size;
    unsigned long available;
} mem_block_info;

static mem_block_info* free_block_head = 0;

void* kmalloc(unsigned long size);
void kfree(void* ptr);

#endif //__ALLOC_H__

#include <utility/alloc.h>
#include <kernel/paging.h>

void* kmalloc(unsigned long size)
{
    kassert(size > 0);
}

void kfree(void* ptr)
{
}



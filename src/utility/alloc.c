#include <utility/alloc.h>
#include <kernel/paging.h>
#include <utility/utility.h>

void list_add(mem_block_info* ptr)
{
    mem_block_info* current = block_head;
    if(block_head == NULL)
    {
        block_head = ptr;
        block_head->next = 0;
        return;
    }

    while(current)
    {
        if(!current->next)
        {
            current->next = ptr;
            ptr->next = 0;
            break;
        }
        current = current->next;
    }

}

mem_block_info* get_mem_block(void* ptr)
{
    kassert(ptr != NULL);

    mem_block_info* block = (unsigned long)ptr - MEM_BLOCK_SIZE;
    return block;
}

mem_block_info* find_free_block(unsigned long size)
{
    mem_block_info* current = block_head;
    while(current)
    {
        if(current->available && current->size >= size)
        {
            break;
        }

        current = current->next;
    }

    return current;
}

mem_block_info* request_memory(unsigned long size)
{
    kassert(size > 0);

    unsigned long total_size = size + MEM_BLOCK_SIZE;
    unsigned long pages_needed = total_size/PAGE_SIZE;
    unsigned long remainder = total_size % PAGE_SIZE;
    if(remainder != 0)
        pages_needed += 1;

    printk("Pages: %d",pages_needed);

    unsigned long remaining_size = (pages_needed * PAGE_SIZE) - total_size;
    void* block_ptr = get_pages(pages_needed);
    mem_block_info* block = block_ptr;

    // If Remaining Size is Greater than Block size we can use it later
    if(remaining_size > MEM_BLOCK_SIZE)
        block->size = size;
    else
        block->size = size + remaining_size; 

    block->available = FALSE;
    block->magic = ALLOCATED;
    list_add(block);

    // Add Remaining Bytes to Free Blocks
    if(remaining_size > MEM_BLOCK_SIZE)
    {
        mem_block_info* remaining_block = (unsigned long)block_ptr + size + MEM_BLOCK_SIZE;
        remaining_block->available = TRUE;
        remaining_block->size = (remaining_size - MEM_BLOCK_SIZE);
        remaining_block->magic = UNALLOCATED;
        list_add(remaining_block);
        printk("Remaining Block Added to List, Size%d\n", remaining_block->size);
    }

    return block;
}

void* kmalloc(unsigned long size)
{
    kassert(size > 0);

    mem_block_info* block = NULL;

    if(block_head)
    {
        block = find_free_block(size);
        if(block)
        {
            block->available = FALSE;
            block->magic = ALLOCATED;
            return ((unsigned long)block + MEM_BLOCK_SIZE);
        }
        else
        {
            block = request_memory(size);
            if(block)
            {
                return ((unsigned long)block + MEM_BLOCK_SIZE);
            }
         }
    }
    else
    {
        block = request_memory(size);
        if(block)
        {
            return ((unsigned long)block + MEM_BLOCK_SIZE);
        }
    }

    return NULL;
}

void kfree(void* ptr)
{
    kassert(ptr != NULL);

    mem_block_info* block = get_mem_block(ptr);

    kassert(block->magic == ALLOCATED);

    block->available = TRUE;
    block->magic = UNALLOCATED;

    printk("Removed Memory At: %x\n",ptr);
}



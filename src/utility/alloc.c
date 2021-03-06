#include <utility/alloc.h>
#include <kernel/paging.h>
#include <utility/utility.h>

void list_add_mem_block(mem_block_info* ptr)
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

void list_remove_mem_block(mem_block_info* ptr)
{
    mem_block_info* current = block_head;
    if(block_head == ptr)
    {
        block_head = block_head->next;
        return;
    }

    while(current)
    {
        if(current->next == ptr)
        {
            current->next = ptr->next;
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

    unsigned long remaining_size = (pages_needed * PAGE_SIZE) - total_size;
    void* block_ptr = get_pages(pages_needed);
    mem_block_info* block = block_ptr;
    memset(block, 0, MEM_BLOCK_SIZE);

    // Split Function will handle the rest of memory
    block->size = size + remaining_size; 

    block->available = FALSE;
    block->magic = ALLOCATED;
    list_add_mem_block(block);

    // Add Remaining Bytes to Free Blocks
    if(remaining_size > MEM_BLOCK_SIZE)
    {
        split_mem(block, remaining_size);
    }

    return block;
}

void* kmalloc(unsigned long size)
{
    kassert(size > 0);

    mem_block_info* block = NULL;

    // Is Block Head Set ?
    if(block_head)
    {
        block = find_free_block(size);
        // If we find a free block return it
        if(block)
        {
            // Can we hold another memory block ? then split
            if(block->size > size + MEM_BLOCK_SIZE)
            {
                split_mem(block, block->size - size);
            }

            block->available = FALSE;
            block->magic = ALLOCATED;
            return ((unsigned long)block + MEM_BLOCK_SIZE);
        }
        else
        {
            // If there is no free block, Request Memory from the Kernel
            block = request_memory(size);
            if(block)
            {
                return ((unsigned long)block + MEM_BLOCK_SIZE);
            }
         }
    }
    else
    {
        // If there is no free block, Request Memory from the Kernel
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

    // Can we Merge ? 
    mem_block_info* modified_block = merge_mem_up(block);
    merge_mem_down(modified_block);
}

void split_mem(mem_block_info* base, unsigned long remaining_size)
{
    // Calculate Remaining Memory after giving space for Mem Block
    unsigned long remaining_memory = remaining_size - MEM_BLOCK_SIZE;

    kassert(remaining_memory >= 1);

    // Adjust Base Size to get rid of Remaining Size
    base->size = base->size - remaining_size;
    mem_block_info* new_block = (unsigned long)base + MEM_BLOCK_SIZE + base->size;
    memset(new_block, 0, MEM_BLOCK_SIZE);

    new_block->size = remaining_memory;
    new_block->merge_prev = base;
    base->merge_next = new_block;
    new_block->available = TRUE;
    new_block->magic = UNALLOCATED;

//    printk("Split! : Base:%x, Base Size:%d | New:%x, New Size:%d\n",base,base->size,new_block,new_block->size);
    // Add New Block to the List
    list_add_mem_block(new_block);
}

mem_block_info* merge_mem_up(mem_block_info* current)
{
    // If there is no mergable block return
    if(current->merge_prev == NULL)
        return current;

    unsigned long current_size = current->size;

    // Get Previous Block
    mem_block_info* prev_block = current->merge_prev;
    // If Previous Block is in use then don't merge
    if(prev_block->available == FALSE)
        return current;

    prev_block->size = prev_block->size + MEM_BLOCK_SIZE + current->size;
    mem_block_info* next_ref_block = current->merge_next;
    if(next_ref_block != NULL)
    {
        next_ref_block->merge_prev = prev_block;
    }
    prev_block->merge_next = next_ref_block;

    list_remove_mem_block(current);

    //printk("Merged Memory: %x->%x, Size:%d, Merge Size:%d \n", (unsigned long)current,prev_block, current_size, prev_block->size);

    return merge_mem_up(prev_block);
}

mem_block_info* merge_mem_down(mem_block_info* current)
{
    // If Current is Null Return
    if(current == NULL)
        return NULL;

    // Recursively Get Next Block
    mem_block_info* next_block = merge_mem_down(current->merge_next);
    if(next_block == NULL)
        return current;

    // If Next Block is not available to merge return current
    if(next_block->available == FALSE)
        return current;

    unsigned long current_size = current->size;

    // Increase the size merging the next block
    current->size = current->size + MEM_BLOCK_SIZE + next_block->size;
    mem_block_info* next_ref_block = next_block->merge_next;
    if(next_ref_block != NULL)
    {
        next_ref_block->merge_prev = current;
    }
    current->merge_next = next_ref_block;
    list_remove_mem_block(next_block);

    //printk("Merged Memory: %x->%x, Size:%d, Merge Size:%d Merge Next:%x\n", next_block,current, current_size, current->size, current->merge_next);

    return current;
}

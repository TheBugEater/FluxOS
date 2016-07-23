#include <mm.h>
#include <paging.h>
#include <terminal.h>

unsigned long round_next_page(unsigned long address)
{
    unsigned long next_addr = 0;
    next_addr = (address + PAGE_SIZE) & 0xFFFFF000;
    return next_addr;
}

unsigned long round_previous_page(unsigned long address)
{
    unsigned long next_addr = 0;
    next_addr = (address - PAGE_SIZE) & 0xFFFFF000;
    return next_addr;
}

void install_mm(kernel_boot_info_t* info)
{
    // Convert it to Real Physical Address, We are in Higher Half Kernel
    unsigned long kernel_end = info->kernel_end - KERNEL_VIRTUAL_BASE;
    phys_alloc_start = round_next_page(kernel_end);
    printk("Allocation Starts at %x\n", phys_alloc_start);

    unsigned long mem_size = info->mbi->mem_upper * 1024;
    printk("Available Physical Memory : %dmb\n", mem_size >> 20);

    unsigned long last_page = round_previous_page(mem_size);
    printk("Last Block : %x\n", last_page);

    num_blocks = (last_page - (unsigned long)phys_alloc_start)/PAGE_SIZE;

    // Allocate memory for Mem Map and Make it Virtual
    mem_map = (unsigned long)phys_alloc_start + KERNEL_VIRTUAL_BASE;
    mem_map_size = num_blocks/(BITS_PER_BYTE*4);
    
    // set new allocation start Point
    phys_alloc_start += mem_map_size;
    printk("Memory Map Allocation Ends at %x\n", phys_alloc_start);

    // Make sure it starts on a new Block
    phys_alloc_start = round_next_page(phys_alloc_start);
    printk("Real Allocation Starts at %x\n", phys_alloc_start);

    num_blocks = (last_page - (unsigned long)phys_alloc_start)/PAGE_SIZE;
    printk("Number of Blocks: %d\n", num_blocks);

    printk("Mem Map: %x\n", mem_map);
    memset(mem_map, 0, mem_map_size * sizeof(unsigned long));
}

void* new_block()
{
    for(int i = 0; i < mem_map_size; i++)
    {
        unsigned long current = mem_map[i];
        for(int j = 0; j < 4 * BITS_PER_BYTE; j++)
        {
            // Is the Block Empty
            if(!BIT_CHECK(current, j))
            {
                BIT_SET(current, j);
                mem_map[i] = current;
                unsigned int frame = (i * 4 * BITS_PER_BYTE + j);
                unsigned long* address = ((unsigned long)phys_alloc_start) + (frame * PAGE_SIZE);
                printk("Found Empty Block At frame=%d Address:%x\n",frame, address);
                return address;
            }
        }
    }

    return 0;
}

void remove_block(void* ptr)
{
    int frame = (((unsigned long)ptr - (unsigned long)phys_alloc_start) & 0xFFFFF000 ) / PAGE_SIZE;

    unsigned long mem_map_offset = frame / (BITS_PER_BYTE * 4);
    unsigned short bit_offset = frame % (BITS_PER_BYTE * 4);

    BIT_CLEAR(mem_map[mem_map_offset], bit_offset);
    printk("Removed Block At frame=%d Address:%x\n",frame, ptr);
}



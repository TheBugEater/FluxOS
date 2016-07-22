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
    alloc_start = round_next_page(info->kernel_end);
    printk("Allocation Starts at %x\n", alloc_start);

    unsigned long mem_size = info->mbi->mem_upper * 1024;
    printk("Available Physical Memory : %dmb\n", mem_size >> 20);

    unsigned long last_page = round_previous_page(mem_size);
    printk("Last Block : %x\n", last_page);

    num_blocks = (last_page - (unsigned long)alloc_start)/PAGE_SIZE;

    // Allocate memory for Mem Map
    mem_map = alloc_start;
    mem_map_size = num_blocks/(BITS_PER_BYTE*4);
    
    // set new allocation start Point
    alloc_start = mem_map + mem_map_size;
    printk("Memory Map Allocation Ends at %x\n", alloc_start);

    // Make sure it starts on a new Block
    alloc_start = round_next_page(alloc_start);
    printk("Real Allocation Starts at %x\n", alloc_start);

    num_blocks = (last_page - (unsigned long)alloc_start)/PAGE_SIZE;
    printk("Number of Blocks: %d\n", num_blocks);

    memset(mem_map, 0, mem_map_size * sizeof(unsigned long));

    printk("##########################\n");
    printk("### TESTING ALLOCATION ###\n");
    printk("##########################\n");

    char* p = (char*)new_block();
    p = "Hello Wolrd!, I'm at a Dynamically Allocated Block\n";
    printk("%s",p);

    /*
    for(int i=0; i<num_blocks;i++)
    {
        void* p = new_block();
    }
    */
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
                unsigned long* address = ((unsigned long)alloc_start) + (frame * PAGE_SIZE);
                printk("Found Empty Block At frame=%d Address:%x\n",frame, address);
                return address;
            }
        }
    }

    return 0;
}

void remove_block(void* ptr)
{
    int frame = (((unsigned long)ptr - (unsigned long)alloc_start) & 0xFFFFF000 ) / PAGE_SIZE;

    unsigned long mem_map_offset = frame / (BITS_PER_BYTE * 4);
    unsigned short bit_offset = frame % (BITS_PER_BYTE * 4);

    BIT_CLEAR(mem_map[mem_map_offset], bit_offset);
    printk("Removed Block At frame=%d Address:%x\n",frame, ptr);
}



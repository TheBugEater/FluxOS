#include <kernel/initrd.h>
#include <kernel/paging.h>
#include <boot/multiboot.h>

void initialize_initrd(kernel_boot_info_t* info)
{
    if(info->mbi->mods_count <= 0)
        return;

    multiboot_module_t* mods = ADDR_TO_KERNEL_BASE(info->mbi->mods_addr);
    unsigned long* initrd_addr = ADDR_TO_KERNEL_BASE(mods[0].mod_start);
    unsigned long* initrd_addr_end = ADDR_TO_KERNEL_BASE(mods[0].mod_end);

    printk("Ramdisk Start Location: %x\n", initrd_addr);
    printk("Ramdisk End Location: %x\n", initrd_addr_end);
    unsigned int file_count = *initrd_addr;
    printk("File Count: %u | Mods Count:%d\n", file_count, info->mbi->mods_count);

    unsigned long file_offset = sizeof(unsigned int);
    
//    file_block* file_blocks = kmalloc(sizeof(file_block) * file_count);
//    memcpy(file_blocks, initrd_addr + file_offset, sizeof(file_block) * file_count);
    file_block* block = initrd_addr + file_offset;
    printk("Block Addr:%x\n", block);
    printk("Name:%s\n", block->name);
    printk("Start Offset:0x%x\n", block->start_offset);
    printk("Block Size:%d\n", block->block_size);
}

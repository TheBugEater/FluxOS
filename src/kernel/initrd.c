#include <kernel/initrd.h>
#include <kernel/terminal.h>
#include <kernel/paging.h>
#include <boot/multiboot.h>

void initialize_initrd(kernel_boot_info_t* info)
{
    if(info->mbi->mods_count <= 0)
        return;

    multiboot_module_t* mods = ADDR_TO_KERNEL_BASE(info->mbi->mods_addr);
    unsigned long* initrd_addr = ADDR_TO_KERNEL_BASE(mods[0].mod_start);
    unsigned long* initrd_addr_end = ADDR_TO_KERNEL_BASE(mods[0].mod_end);
    
    file_count = *initrd_addr;

    unsigned long file_offset = (unsigned long)initrd_addr + sizeof(unsigned int);
    
    files = kmalloc(sizeof(file_block) * file_count);
    memcpy(files, file_offset, sizeof(file_block) * file_count);
    file_offset += (sizeof(file_block) * file_count);

    unsigned long file_buffer_size = (unsigned long)initrd_addr_end - file_offset;
    memcpy(file_buffer, file_offset, file_buffer_size); 

    list_all_files();
}

void list_all_files()
{
    setcolor(COLOR_GREEN, COLOR_BLACK);
    printk("Ramdisk Files | File Count: %u\n", file_count);
    setcolor(COLOR_BROWN, COLOR_BLACK);
    for(unsigned int i=0; i < file_count; i++)
    {
        printk("%s\n", files[i].name);
    }
    setcolor(COLOR_LIGHT_GREY, COLOR_BLACK);
}

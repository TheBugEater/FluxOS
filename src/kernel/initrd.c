#include <kernel/initrd.h>
#include <kernel/paging.h>
#include <boot/multiboot.h>

void initialize_initrd(kernel_boot_info_t* info)
{
    if(info->mbi->mods_count <= 0)
        return;

    multiboot_module_t* mods = add_page_mapping(info->mbi->mods_addr, 0xDDDD0000);
    unsigned long* initrd_addr = add_page_mapping(mods[1].mod_start, 0xDDDC0000);

    printk("File Addr: %x\n", initrd_addr);
    unsigned int file_count = *(unsigned int*)add_page_mapping(*initrd_addr, 0xEEEE0000);
    printk("File Count: %u | Mods Count:%d\n", file_count, info->mbi->mods_count);
}

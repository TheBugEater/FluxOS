#include "terminal.h"
#include "idt.h"
#include "gdt.h"
#include "utility.h"
#include "keyboard.h"
#include "paging.h"
#include <multiboot.h>

struct kernel_boot_info
{
    unsigned long kernel_end;
    unsigned long kernel_start;
    unsigned long magic_number;
    multiboot_info_t* mbi;

}__attribute__((packed));

void kmain(struct kernel_boot_info info)
{
    if(info.magic_number != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        printk("Invalid Boot Image!");
        return;
    }
    else
    {
        printk("Boot Successfull!");
    }

    terminal_initialize();

    // Disable Interrupts
    disable_interrupts();

    install_gdt();
    install_idt();

    // Enable Interrupts
    enable_interrupts();

    install_keyboard();
   
    while(1);
}

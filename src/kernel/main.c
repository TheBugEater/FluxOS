#include "terminal.h"
#include "idt.h"
#include "gdt.h"
#include "utility.h"
#include "keyboard.h"
#include "paging.h"
#include <mm.h>

void kmain(kernel_boot_info_t info)
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

    install_mm(&info);
   
    while(1);
}

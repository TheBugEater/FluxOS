#include <kernel/terminal.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/keyboard.h>
#include <kernel/pit.h>
#include <kernel/paging.h>
#include <kernel/mm.h>
#include <utility/utility.h>
#include <utility/alloc.h>
#include <utility/list.h>

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

    install_mm(&info);
    install_paging(&info);
 
    install_keyboard();
    install_pit();
  
    initialize_initrd(&info);

    while(1);
}

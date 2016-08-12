OUTPUT_FORMAT("elf32-i386")
ENTRY(start)

phys = 0x100000;
offset = 0xC0000000;
virt = offset + phys;

SECTIONS
{
    . = virt; /* the code should be loaded at 1 MB */

    kernel_start = .;

    .text ALIGN (0x1000) : AT(ADDR(.text) - offset)
    {
        *(.text)
    }

    /* align at 4 KB */
    /* all text sections from all files */
    .rodata ALIGN (0x1000) :AT(ADDR(.rodata) - offset)
 /* align at 4 KB */
    {
        *(.rodata*)
        /* all read-only data sections from all files */
    }

    .data ALIGN (0x1000) :AT(ADDR(.data) - offset)
    {
        *(.data)
    } /* align at 4 KB */

    .bss ALIGN (0x1000) : AT(ADDR(.bss) - offset)
    {
        *(COMMON)
        *(.bss)
    } /* align at 4 KB */
    /* all data sections from all files */
    /* all COMMON sections from all files */
    /* all bss sections from all files */
    kernel_end = .;
}

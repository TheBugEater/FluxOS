OUTPUT_FORMAT("elf32-i386")
ENTRY(start)
SECTIONS
{
    . = 0x00100000; /* the code should be loaded at 1 MB */

    .text ALIGN (0x1000) :
    {
        *(.text)
    }

    /* align at 4 KB */
    /* all text sections from all files */
    .rodata ALIGN (0x1000) : /* align at 4 KB */
    {
        *(.rodata*)
        /* all read-only data sections from all files */
    }

    .data ALIGN (0x1000) :
    {
        *(.data)
    } /* align at 4 KB */

    .bss ALIGN (0x1000) :
    {
        *(COMMON)
        *(.bss)
    } /* align at 4 KB */
    /* all data sections from all files */
    /* all COMMON sections from all files */
    /* all bss sections from all files */
}

SECTION .text

global inb
global outb
global disable_interrupts
global enable_interrupts

inb:
    mov edx, [esp + 4]
    in  al, dx
    ret

outb:
    mov edx, [esp + 4]
    mov al, [esp + 4 + 4]
    out dx, al
    ret

disable_interrupts:
    cli
    ret

enable_interrupts:
    sti
    ret

SECTION .data

SECTION .bss

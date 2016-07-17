SECTION .text

global inb
global outb
global disable_irq
global enable_irq

inb:
    mov edx, [esp + 4]
    in  al, dx
    ret

outb:
    mov edx, [esp + 4]
    mov al, [esp + 4 + 4]
    out dx, al
    ret

disable_irq:
    cli
    ret

enable_irq:
    sti
    ret

SECTION .data

SECTION .bss

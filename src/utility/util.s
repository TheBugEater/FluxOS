SECTION .text

global inb
global outb
global disable_interrupts
global enable_interrupts
global get_cr0
global write_cr0
global get_cr3
global write_cr3
 
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

get_cr0:
    mov eax, cr0
    ret

write_cr0:
    mov eax, [esp + 4]
    mov cr0, eax
    ret

get_cr3:
    mov eax, cr3
    ret

write_cr3:
    mov eax, [esp + 4]
    mov cr3, eax
    ret

SECTION .data

SECTION .bss

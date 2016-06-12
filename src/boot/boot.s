section .text
    align 4
    dd 0x1BADB002
    dd 0x00
    dd - (0x1BADB002 + 0x00)

global start
global inb
global outb

extern kmain

start:
    cli
    mov esp, stack_memory
    call kmain
    hlt

inb:
    mov edx, [esp + 4]
    in al, dx
    ret

outb:
    mov edx, [esp + 4]
    mov al, [esp + 4 + 4]
    out dx, al
    ret

section .bss
resb 8192
stack_memory:

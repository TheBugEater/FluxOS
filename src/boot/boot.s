section .text
    align 4
    dd 0x1BADB002
    dd 0x00
    dd - (0x1BADB002 + 0x00)

global start

extern kmain

start:
    cli
    mov esp, stack_memory
    call kmain
    hlt

section .bss
resb 8192
stack_memory:

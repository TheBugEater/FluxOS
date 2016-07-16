MULTIBOOT_MAGIC_NUMBER equ 0x1BADB002
MULTIBOOT_FLAGS equ 0x0
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC_NUMBER + MULTIBOOT_FLAGS)

SECTION .text
; Define Magic Boot Header for Grub
align 4
    dd MULTIBOOT_MAGIC_NUMBER
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

global start
extern kmain

start:
    cli
    mov esp, StackPointer
    call kmain
    jmp $

SECTION .data

SECTION .bss
; Define an 8KB Stack
resb 8092
StackPointer:


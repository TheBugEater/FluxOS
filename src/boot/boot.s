extern kernel_start
extern kernel_end

MULTIBOOT_MAGIC_NUMBER      equ 0x1BADB002
MULTIBOOT_PAGE_ALIGN        equ 1<<0
MULTIBOOT_MEMORY_INFO       equ 1<<1
MULTIBOOT_FLAGS             equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
MULTIBOOT_CHECKSUM          equ -(MULTIBOOT_MAGIC_NUMBER + MULTIBOOT_FLAGS)

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
    push ebx
    push eax
    push kernel_start
    push kernel_end
    call kmain
    jmp $

SECTION .data

SECTION .bss
; Define an 16KB Stack
resb 16384
StackPointer:


extern kernel_start
extern kernel_end

MULTIBOOT_MAGIC_NUMBER      equ 0x1BADB002
MULTIBOOT_PAGE_ALIGN        equ 1<<0
MULTIBOOT_MEMORY_INFO       equ 1<<1
MULTIBOOT_FLAGS             equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
MULTIBOOT_CHECKSUM          equ -(MULTIBOOT_MAGIC_NUMBER + MULTIBOOT_FLAGS)

; Kernel Base location of the Virtual Memory Space
KERNEL_VIRTUAL_BASE equ 0xC0000000

; Page Directory ID of the Kernel's PDE
KERNEL_PAGE_NUM     equ (KERNEL_VIRTUAL_BASE >> 22)

SECTION .text
; Define Magic Boot Header for Grub
align 4
    dd MULTIBOOT_MAGIC_NUMBER
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

global start
extern kmain

start:
    mov ecx, (boot_time_pages - KERNEL_VIRTUAL_BASE)
    mov cr3, ecx    ; Load page directory

    mov ecx, cr4
    or ecx, 0x00000010  ; Set PSE bit in CR4 to enable 4MB pages
    mov cr4, ecx

    mov ecx, cr0
    or ecx, 0x80000000  ; Set PG bit in CR0 to enable paging
    mov cr0, ecx

    ; EIP currently holds physical address, so we need a long jump to
    ; the correct virtual address to continue execution in kernel space
    lea ecx, [virtual_kernel]
    jmp ecx     ; Absolute jump!!

virtual_kernel:
    mov dword [boot_time_pages], 0
    invlpg [0] ; Unmap Identity Mappings on first 4mb
    cli
    mov esp, StackPointer
    add ebx, KERNEL_VIRTUAL_BASE
    push ebx
    push eax
    push kernel_start
    push kernel_end
    call kmain
    jmp $

SECTION .data
align 4096
boot_time_pages:
    dd 0x00000083   ; First 4MB, which will be unmapped later
    times (KERNEL_PAGE_NUM - 1) dd 0    ; Pages before kernel
    dd 0x00000083   ; Kernel 4MB at 3GB offset
    times (1024 - KERNEL_PAGE_NUM - 1) dd 0 ; Pages after kernel

SECTION .bss
; Define a 16KB Stack
resb 16384
StackPointer:


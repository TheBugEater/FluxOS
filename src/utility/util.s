SECTION .text

global inb
global outb
global disable_interrupts
global enable_interrupts
global get_page_fault_addr
global get_cr0
global write_cr0
global get_cr3
global write_cr3
global switch_page_directory
global invalidate_addr

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

get_page_fault_addr:
    mov eax, cr2
    ret

switch_page_directory:
    mov eax, [esp + 4]
    mov ebx, cr3
    and ebx, 0FFFFF000h
    or  eax, ebx
    mov cr3, eax

    mov eax, cr0
    or  eax, 080000000h
    mov cr0, eax
    ret

invalidate_addr:
    mov eax, [esp + 4]
    invlpg[eax]
    ret

SECTION .data

SECTION .bss

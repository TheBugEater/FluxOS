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
    jmp $ 

inb:
    mov edx, [esp + 4]
    in al, dx
    ret

outb:
    mov edx, [esp + 4]
    mov al, [esp + 4 + 4]
    out dx, al
    ret

global idt_load
idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

global gdt_flush    
gdt_flush:
    mov eax, [esp + 4]
    lgdt [eax]     
    mov ax, 0x10 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2 
flush2:
    ret            

section .bss
resb 8192
stack_memory:

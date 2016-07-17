SECTION .text

extern kernel_idt_handler

global load_idt
load_idt:
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
    jmp 0x08:flushjump 
flushjump:
    ret

;##########################################
;   Interrupt Descriptors #################
%macro isr_with_error_code 1
global isr_%1
isr_%1:
    cli
    push dword %1
    jmp idt_handle
%endmacro

%macro isr_without_error_code 1
global isr_%1
isr_%1:
    cli
    push dword 0
    push dword %1
    jmp idt_handle
%endmacro

%macro irq_handle 2
global irq_%1
irq_%1:
    cli
    push dword 0
    push dword %2
    jmp idt_handle
%endmacro

isr_without_error_code 0
isr_without_error_code 1
isr_without_error_code 2
isr_without_error_code 3
isr_without_error_code 4
isr_without_error_code 5
isr_without_error_code 6
isr_without_error_code 7
isr_with_error_code 8
isr_without_error_code 9
isr_with_error_code 10
isr_with_error_code 11
isr_with_error_code 12
isr_with_error_code 13
isr_with_error_code 14
isr_without_error_code 15
isr_without_error_code 16
isr_with_error_code 17
isr_without_error_code 18
isr_without_error_code 19
isr_without_error_code 20
isr_without_error_code 21
isr_without_error_code 22
isr_without_error_code 23
isr_without_error_code 24
isr_without_error_code 25
isr_without_error_code 26
isr_without_error_code 27
isr_without_error_code 28
isr_without_error_code 29
isr_without_error_code 30
isr_without_error_code 31

irq_handle 0, 32
irq_handle 1, 33
irq_handle 2, 34
irq_handle 3, 35
irq_handle 4, 36
irq_handle 5, 37
irq_handle 6, 38
irq_handle 7, 39
irq_handle 8, 40
irq_handle 9, 41
irq_handle 10, 42
irq_handle 11, 43
irq_handle 12, 44
irq_handle 13, 45
irq_handle 14, 46
irq_handle 15, 47

idt_handle:
    pushad
    call kernel_idt_handler
    popad
    add esp, 8
    iretd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;#############################################

SECTION .data


SECTION .bss

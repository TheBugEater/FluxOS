SECTION .text
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

;  0: Divide By Zero Exception
isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_handler

;  1: Debug Exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_handler

;  2: Non Maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_handler

;  3: Int 3 Exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_handler

;  4: INTO Exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_handler

;  5: Out of Bounds Exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_handler

;  6: Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_handler

;  7: Coprocessor Not Available Exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_handler

;  8: Double Fault Exception (With Error Code!)
isr8:
    cli
    push byte 8
    jmp isr_handler

;  9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_handler

; 10: Bad TSS Exception (With Error Code!)
isr10:
    cli
    push byte 10
    jmp isr_handler

; 11: Segment Not Present Exception (With Error Code!)
isr11:
    cli
    push byte 11
    jmp isr_handler

; 12: Stack Fault Exception (With Error Code!)
isr12:
    cli
    push byte 12
    jmp isr_handler

; 13: General Protection Fault Exception (With Error Code!)
isr13:
    cli
    push byte 13
    jmp isr_handler

; 14: Page Fault Exception (With Error Code!)
isr14:
    cli
    push byte 14
    jmp isr_handler

; 15: Reserved Exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_handler

; 16: Floating Point Exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_handler

; 17: Alignment Check Exception
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_handler

; 18: Machine Check Exception
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_handler

; 19: Reserved
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_handler

; 20: Reserved
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_handler

; 21: Reserved
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_handler

; 22: Reserved
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_handler

; 23: Reserved
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_handler

; 24: Reserved
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_handler

; 25: Reserved
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_handler

; 26: Reserved
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_handler

; 27: Reserved
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_handler

; 28: Reserved
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_handler

; 29: Reserved
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_handler

; 30: Reserved
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_handler

; 31: Reserved
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_handler

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mapping IRQ0 to IRQ15 from IDT Entry 32 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

irq0:
    cli
    push byte 0
    push byte 32
    jmp isr_handler

irq1:
    cli
    push byte 0
    push byte 33
    jmp isr_handler


irq2:
    cli
    push byte 0
    push byte 34
    jmp isr_handler


irq3:
    cli
    push byte 0
    push byte 35
    jmp isr_handler


irq4:
    cli
    push byte 0
    push byte 36
    jmp isr_handler


irq5:
    cli
    push byte 0
    push byte 37
    jmp isr_handler


irq6:
    cli
    push byte 0
    push byte 38
    jmp isr_handler


irq7:
    cli
    push byte 0
    push byte 39
    jmp isr_handler


irq8:
    cli
    push byte 0
    push byte 40
    jmp isr_handler


irq9:
    cli
    push byte 0
    push byte 41
    jmp isr_handler


irq10:
    cli
    push byte 0
    push byte 42
    jmp isr_handler


irq11:
    cli
    push byte 0
    push byte 43
    jmp isr_handler


irq12:
    cli
    push byte 0
    push byte 44
    jmp isr_handler


irq13:
    cli
    push byte 0
    push byte 45
    jmp isr_handler


irq14:
    cli
    push byte 0
    push byte 46
    jmp isr_handler

irq15:
    cli
    push byte 0
    push byte 47
    jmp isr_handler



; Interupt Service Routines 
global isr_handler
extern kernel_isr_handler

isr_handler:
    call kernel_isr_handler
    add esp, 8
    iret

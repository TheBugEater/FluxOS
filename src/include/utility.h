#ifndef __UTILITY_H__
#define __UTILITY_H__ 

#include <multiboot.h>

typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long      uint64_t;
 
extern char inb(unsigned short port);
extern void outb(unsigned short port, char ch);

extern void disable_interrupts(void);
extern void enable_interrupts(void);

extern unsigned int get_cr0();
extern unsigned int get_cr3();

extern void write_cr0(unsigned int value);
extern void write_cr3(unsigned int value);

void* memcpy(void* dest, const void* src, int count);

void* memset(void* dest, unsigned char val, int count);

void kernel_assert(unsigned int condition);

struct kernel_boot_info
{
    unsigned long kernel_end;
    unsigned long kernel_start;
    unsigned long magic_number;
    multiboot_info_t* mbi;

}__attribute__((packed));
typedef struct kernel_boot_info kernel_boot_info_t;

#endif

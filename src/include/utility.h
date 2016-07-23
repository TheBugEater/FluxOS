#ifndef __UTILITY_H__
#define __UTILITY_H__ 

#include <multiboot.h>

#define TRUE    1
#define FALSE   0

#define BITS_PER_BYTE   8
#define KERNEL_VIRTUAL_BASE 0xC0000000
#define KERNEL_DIR_INDEX (KERNEL_VIRTUAL_BASE >> 22)

#define BIT_CHECK(val, bit)     ((val) & 1 << bit)
#define BIT_SET(val, bit)       val |= 1 << bit
#define BIT_CLEAR(val, bit)       val &= ~(1 << bit)

#define VIRTUAL_ADDR_DIR(x) ((unsigned long)x >> 22)
#define VIRTUAL_ADDR_PAGE(x) (((unsigned long)x >> 12) & 0x3FF)
#define VIRTUAL_ADDR_OFFSET(x) ((unsigned long)x & 0x3FF)
#define ADDR_TO_KERNEL_BASE(x) ((unsigned long)x + KERNEL_VIRTUAL_BASE)

typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long      uint64_t;
typedef unsigned char           BOOL;
 
extern char inb(unsigned short port);
extern void outb(unsigned short port, char ch);

extern void disable_interrupts(void);
extern void enable_interrupts(void);

extern unsigned int get_page_fault_addr();
extern void switch_page_directory(unsigned long value);
extern void invalidate_addr(unsigned long* addr);

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

#ifndef __UTILITY_H__
#define __UTILITY_H_

extern char inb(unsigned short port);
extern void outb(unsigned short port, char ch);

extern void disable_interrupts(void);
extern void enable_interrupts(void);

void* memcpy(void* dest, const void* src, int count);

void* memset(void* dest, unsigned char val, int count);

void kernel_assert(unsigned int condition);

#endif

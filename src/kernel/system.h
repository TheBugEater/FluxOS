#ifndef __SYSTEM_H__
#define __SYSTEM_H__

extern char inb(unsigned short port);
extern void outb(unsigned short port, char ch);

void* memcpy(void* dest, const void* src, int count);

void* memset(void* dest, unsigned char val, int count);


#endif

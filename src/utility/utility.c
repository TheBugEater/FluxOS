#include <utility/utility.h>

void* memcpy(void* dest, const void* src, int count)
{
    unsigned char* destref = (unsigned char*)dest;

    for(int i=0; i<count; i++)
    {
        destref[i] = ((unsigned char*)src)[i];
    }

    return dest;
}

void* memset(void* dest, unsigned char val, int count)
{
    unsigned char* destref = (unsigned char*)dest;

    for(int i=0; i<count; i++)
    {
        destref[i] = val;
    }

    return dest;
}

void kernel_assert(unsigned int condition)
{
    if(condition == 0)
    {
        __asm__ __volatile__ ("int $129");
    }
}

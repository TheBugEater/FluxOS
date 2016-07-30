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


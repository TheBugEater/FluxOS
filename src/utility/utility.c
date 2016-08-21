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

char* strcpy(char *strDest, const char *strSrc)
{
    kassert(strDest!=NULL && strSrc!=NULL);
    char *temp = strDest;
    while(*strDest++ = *strSrc++); 
    return temp;
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

int strlen(const char* str)
{
    unsigned long count = 0;
    while(*str++)
    {
        count++;
    }
    return count;
}

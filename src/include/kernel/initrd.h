#ifndef __INITRD_H__
#define __INITRD_H__ 

#include <utility/utility.h>

typedef struct 
{
    char name[128];
    unsigned int start_offset;
    unsigned int block_size;
} file_block;

void initialize_initrd(kernel_boot_info_t* info);
void list_all_files();

file_block* files;
unsigned int file_count;
char* file_buffer;

#endif //__INITRD_H__

#ifndef __INITRD_H__
#define __INITRD_H__ 

#include <utility/utility.h>
#include <kernel/vfs.h>

typedef struct 
{
    char name[128];
    unsigned int start_offset;
    unsigned int block_size;
} file_block;

void initialize_initrd(kernel_boot_info_t* info);

unsigned long read_initrd(vfs_node_t *node, unsigned long offset, unsigned long size, unsigned char *buffer);
struct dirent *readdir_initrd(vfs_node_t *node, unsigned long index);
vfs_node_t *finddir_initrd(vfs_node_t *node, char *name);

file_block* files;
unsigned int file_count;
char* file_buffer;

vfs_node_t* initrd_file_nodes;

#endif //__INITRD_H__

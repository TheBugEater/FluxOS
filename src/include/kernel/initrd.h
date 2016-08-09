#include <utility/utility.h>

typedef struct 
{
    char name[128];
    unsigned int start_offset;
    unsigned int block_size;
} file_block;

void initialize_initrd(kernel_boot_info_t* info);
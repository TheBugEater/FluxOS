#include <utility/utility.h>

typedef unsigned long (*read_type_t)(struct fs_node*,unsigned long,unsigned long,unsigned char*);
typedef unsigned long (*write_type_t)(struct fs_node*,unsigned long,unsigned long,unsigned char*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);

typedef struct fs_node
{
    char name[128];
    unsigned long length;
    unsigned long flags;

    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
} fs_node_t;

// The File System Root
static fs_node_t* fs_root;

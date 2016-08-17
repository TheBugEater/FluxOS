#include <utility/utility.h>

enum VFSNodeType
{
    VFS_FILE = 1,
    VFS_DIRECTORY = 3
};

typedef unsigned long (*read_type_t)(struct inode*,unsigned long,unsigned long,unsigned char*);
typedef unsigned long (*write_type_t)(struct inode*,unsigned long,unsigned long,unsigned char*);
typedef void (*open_type_t)(struct inode*);
typedef void (*close_type_t)(struct inode*);
typedef struct dirent* (*read_dir_t)(struct inode*, unsigned long);

typedef struct inode
{
    char name[128];
    unsigned long length;
    unsigned long flags;

    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;

    read_dir_t readdir;
} inode_t;

struct dirent
{
    char name[128]; 
    unsigned long ino;     
};

// The File System Root
static inode_t* fs_root;

unsigned long read_fs(inode_t *node, unsigned long offset, unsigned long size, unsigned char *buffer);
unsigned long write_fs(inode_t *node, unsigned long offset, unsigned long size, unsigned char *buffer);
void open_fs(inode_t *node, unsigned char read, unsigned char write);
void close_fs(inode_t *node);
struct dirent *readdir_fs(inode_t *node, unsigned long index);


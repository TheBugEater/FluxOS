#include <utility/utility.h>
#include <utility/list.h>

typedef enum _VFSNodeType
{
    VFS_FILE = 1,
    VFS_DIRECTORY = 2
} VFSNodeType;

typedef unsigned long (*read_type_t)(struct vfs_node*,unsigned long,unsigned long,unsigned char*);
typedef unsigned long (*write_type_t)(struct vfs_node*,unsigned long,unsigned long,unsigned char*);
typedef void (*open_type_t)(struct vfs_node*);
typedef void (*close_type_t)(struct vfs_node*);
typedef struct dirent* (*read_dir_t)(struct vfs_node*, unsigned long);
typedef struct vfs_node* (*finddir_type_t)(struct fs_node*,char *name);

typedef struct vfs_node
{
    char name[128];
    unsigned long length;
    unsigned long index;
    VFSNodeType type;

    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    finddir_type_t finddir;

    read_dir_t readdir;
} vfs_node_t;

typedef struct _dirent
{
    char name[128]; 
    unsigned long ino;     
}dirent;

// The File System Root
static vfs_node_t* fs_root;
static list* mount_points = NULL;

void mount(vfs_node_t* node);
void umount(vfs_node_t* node);

unsigned long read_fs(vfs_node_t *node, unsigned long offset, unsigned long size, unsigned char *buffer);
unsigned long write_fs(vfs_node_t *node, unsigned long offset, unsigned long size, unsigned char *buffer);
void open_fs(vfs_node_t *node, unsigned char read, unsigned char write);
void close_fs(vfs_node_t *node);
struct dirent *readdir_fs(vfs_node_t *node, unsigned long index);
vfs_node_t *finddir_fs(vfs_node_t *node, char *name);

void list_mount_points();

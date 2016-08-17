#include <kernel/vfs.h>
#include <utility/utility.h>

unsigned long read_fs(inode_t *node, unsigned long offset, unsigned long size, unsigned char *buffer)
{
    if(node != NULL && node->read != NULL)
    {
        return node->read(node, offset, size, buffer);
    }

    return NULL;
}

unsigned long write_fs(inode_t *node, unsigned long offset, unsigned long size, unsigned char *buffer)
{
    if(node != NULL && node->write != NULL)
    {
        return node->write(node, offset, size, buffer);
    }

    return NULL;
}

void open_fs(inode_t *node, unsigned char read, unsigned char write)
{
    if(node != NULL && node->open != NULL)
    {
        node->open(node);
    }
}

void close_fs(inode_t *node)
{
    if(node != NULL && node->close != NULL)
    {
        node->close(node);
    }
}

struct dirent *readdir_fs(inode_t *node, unsigned long index)
{
    if(node != NULL && node->readdir != NULL)
    {
        if(BIT_CHECK(node->flags, VFS_DIRECTORY))
        {
            return node->readdir(node, index);
        }
    }
}



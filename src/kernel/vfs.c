#include <kernel/vfs.h>
#include <kernel/terminal.h>
#include <utility/utility.h>

unsigned long read_fs(vfs_node_t *node, unsigned long offset, unsigned long size, unsigned char *buffer)
{
    if(node != NULL && node->read != NULL)
    {
        return node->read(node, offset, size, buffer);
    }

    return NULL;
}

unsigned long write_fs(vfs_node_t *node, unsigned long offset, unsigned long size, unsigned char *buffer)
{
    if(node != NULL && node->write != NULL)
    {
        return node->write(node, offset, size, buffer);
    }

    return NULL;
}

void open_fs(vfs_node_t *node, unsigned char read, unsigned char write)
{
    if(node != NULL && node->open != NULL)
    {
        node->open(node);
    }
}

void close_fs(vfs_node_t *node)
{
    if(node != NULL && node->close != NULL)
    {
        node->close(node);
    }
}

struct dirent *readdir_fs(vfs_node_t *node, unsigned long index)
{
    if(node != NULL && node->readdir != NULL)
    {
        if(node->type == VFS_DIRECTORY)
        {
            return node->readdir(node, index);
        }
    }
}

vfs_node_t *finddir_fs(vfs_node_t *node, char *name)
{
    if(node != NULL && node->finddir != NULL && name != NULL)
    {
        if(node->type == VFS_DIRECTORY)
        {
            return node->finddir(node, name);
        }
    }
}

void mount(vfs_node_t* node)
{
    if(mount_points == NULL)
        mount_points = create_list();

    list_node* node_point = mount_points->next;
    while(node_point)
    {
        vfs_node_t* current_node = (vfs_node_t*)node_point->value;
        if(strcmp(current_node->name, node->name) == 0)
        {
            printk("Mount Point with the same name exists.\n");
            return;
        }
        node_point = node_point->next;
    }

    list_push(mount_points, node);
}

void umount(vfs_node_t* node)
{
    if(mount_points == NULL)
        return;

    list_node* node_point = mount_points->next;
    while(node_point)
    {
        vfs_node_t* current_node = (vfs_node_t*)node_point->value;
        if(strcmp(current_node->name, node->name) == 0)
        {
            list_remove(mount_points, node);
            return;
        }
        node_point = node_point->next;
    }

    printk("Mount Point does not exist!\n");
}

void list_mount_points()
{
    setcolor(COLOR_GREEN, COLOR_BLACK);
    printk("Listing Mount Points\n");
    list_node* node_point = mount_points->next;
    while(node_point)
    {
        setcolor(COLOR_RED, COLOR_BLACK);
        vfs_node_t* current_node = (vfs_node_t*)node_point->value;

        printk("%s\n",current_node->name);
        unsigned int i = 1;
        dirent* current_dirent = NULL;
        while(current_dirent = readdir_fs(current_node, i))
        {
            setcolor(COLOR_BROWN, COLOR_BLACK);
            printk("..%s\n", current_dirent->name);
            vfs_node_t* current_file = finddir_fs(current_node, current_dirent->name);
            if(current_file)
            {
                setcolor(COLOR_DARK_GREY, COLOR_BLACK);
                char buff[1024];
                unsigned long size = read_fs(current_file, 0, 1024, buff);
                buff[size] = 0;
                printk("\n%s\n", buff);
            }
            i++;
        }

        node_point = node_point->next;
    }
    setcolor(COLOR_LIGHT_GREY, COLOR_BLACK);
}

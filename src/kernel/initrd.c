#include <kernel/initrd.h>
#include <kernel/paging.h>
#include <boot/multiboot.h>

// Declaring this as a global var will retain the pointer
dirent var_dirent;

void initialize_initrd(kernel_boot_info_t* info)
{
    if(info->mbi->mods_count <= 0)
        return;

    multiboot_module_t* mods = ADDR_TO_KERNEL_BASE(info->mbi->mods_addr);
    unsigned long* initrd_addr = ADDR_TO_KERNEL_BASE(mods[0].mod_start);
    unsigned long* initrd_addr_end = ADDR_TO_KERNEL_BASE(mods[0].mod_end);
    
    file_count = *initrd_addr;

    unsigned long file_offset = (unsigned long)initrd_addr + sizeof(unsigned int);
    
    files = kmalloc(sizeof(file_block) * file_count);
    memcpy(files, file_offset, sizeof(file_block) * file_count);
    file_offset += (sizeof(file_block) * file_count);

    unsigned long file_buffer_size = (unsigned long)initrd_addr_end - file_offset;
    memcpy(file_buffer, file_offset, file_buffer_size); 

    vfs_node_t* initrd_root = (vfs_node_t*)kmalloc(sizeof(vfs_node_t));
    strcpy(initrd_root->name, "initrd");
    initrd_root->length = 0;
    initrd_root->type = VFS_DIRECTORY;
    initrd_root->index = 0;
    initrd_root->read = NULL;
    initrd_root->write = NULL;
    initrd_root->open = NULL;
    initrd_root->close = NULL;
    initrd_root->readdir = &readdir_initrd;
    initrd_root->finddir = &finddir_initrd;

    // Mount Directory
    mount(initrd_root);

    initrd_file_nodes = (vfs_node_t*)kmalloc(sizeof(vfs_node_t) * file_count);
    memset(initrd_file_nodes, 0, sizeof(vfs_node_t) * file_count);
    for(unsigned int i=0; i < file_count; i++)
    {
        file_block file = files[i];
        strcpy(initrd_file_nodes[i].name, file.name);
        initrd_file_nodes[i].length = file.block_size;
        initrd_file_nodes[i].type = VFS_FILE;
        initrd_file_nodes[i].index = i + 1;
        initrd_file_nodes[i].read = &read_initrd;
        initrd_file_nodes[i].write = NULL;
        initrd_file_nodes[i].open = NULL;
        initrd_file_nodes[i].close = NULL;
        initrd_file_nodes[i].readdir = NULL;
    }
}

unsigned long read_initrd(vfs_node_t *node, unsigned long offset, unsigned long size, unsigned char *buffer)
{
    if(node->index > file_count)
        return 0;

    file_block block = files[node->index - 1];
    if(block.block_size < offset)
        return 0;

    if(offset + size > block.block_size)
        size = block.block_size - offset;

    memcpy(buffer, (unsigned long)file_buffer + block.start_offset, size);
    return size;
}

struct dirent *readdir_initrd(vfs_node_t *node, unsigned long index)
{
    if(node == NULL)
        return NULL;

    memset(&var_dirent, 0, sizeof(dirent));
    if(strcmp(node->name, "initrd") == 0)
    {
        if(index == 0)
        {
            strcpy(var_dirent.name, "initrd");
            var_dirent.ino = 0;
            return &var_dirent;
        }

        if(index > file_count)
        {
            return NULL;
        }

        file_block file = files[index - 1];
        strcpy(var_dirent.name, file.name);
        var_dirent.ino = index;
        return &var_dirent;
    }

    return NULL;
}

vfs_node_t *finddir_initrd(vfs_node_t *node, char *name)
{
    if(node == NULL)
        return NULL;

    if(strcmp(node->name, "initrd") == 0)
    {
        for(unsigned int i=0; i < file_count; i++)
        {
            vfs_node_t* file = &initrd_file_nodes[i];
            if(strcmp(file->name, name) == 0)
            {
                return file;
            }
        }
    }

    return NULL;
}

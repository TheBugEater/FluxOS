#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#define MAX_FILES 32
#define INITRD_DIR "initrd/"

typedef struct 
{
    char name[128];
    unsigned int start_offset;
    unsigned int block_size;
} file_block;

// initrd file
// |4 bytes|file_nodes|actual file data|
// First 4 bytes contain the number of file_blocks

int main(int argc, char *argv[])
{
    int file_count = 0;
    file_block files[MAX_FILES];

    printf("Creating the Ramdisk...\n");

    FILE* fp = fopen("initrd.img", "wb");
    if(!fp)
    {
        return 1;
    }

    struct dirent *file;
    DIR *dir = opendir(INITRD_DIR);
    
    unsigned long buffer_size = 0;

    if(dir)
    {
        printf("Reading Directory...\n");

        while((file = readdir(dir)) != NULL)
        {
            if(file->d_type == DT_REG)
            {
                if(file_count > MAX_FILES)
                    break;

                int root_len = strlen(INITRD_DIR);
                int name_len = strlen(file->d_name);

                char* complete_path = (char*)malloc(root_len + name_len + 1);
                memcpy(complete_path, INITRD_DIR, root_len);
                memcpy(complete_path + root_len, file->d_name, name_len);
                complete_path[root_len + name_len] = '\0';

                FILE* tmpfp = fopen(complete_path, "rb");
                if(!tmpfp)
                {
                    printf("File %s is not Readable\n",file->d_name);
                    return 1;
                }

                free(complete_path);


                file_block block;
                strncpy(block.name, file->d_name, 128);

                fseek(tmpfp, 0, SEEK_END);
                block.block_size = ftell(tmpfp);
                buffer_size += block.block_size;
                fclose(tmpfp);

                files[file_count] = block;
                file_count++;
            }
        }
        closedir(dir);
    }

    char* file_buffer = (char*)malloc(buffer_size);

    unsigned long buffer_offset = 0;

    printf("Packing Files to the Ramdisk...\n");

    for(int i = 0; i < file_count; i++)
    {
        file_block* block = &files[i];
        int root_len = strlen(INITRD_DIR);
        int name_len = strlen(block->name);

        printf("Packing File %s...\n", block->name);
        
        char* complete_path = (char*)malloc(root_len + name_len + 1);
        memcpy(complete_path, INITRD_DIR, root_len);
        memcpy(complete_path + root_len, block->name, name_len);
        complete_path[root_len + name_len] = '\0';

        FILE* current_file = fopen(complete_path, "rb");
        if(!current_file)
        {
            printf("File %s is not Readable\n",file->d_name);
            return 1;
        }
        
        block->start_offset = buffer_offset;

        char* file_content = (char*)malloc(block->block_size);
        fread(file_content, block->block_size, 1, current_file);
        memcpy(file_buffer + buffer_offset, file_content, block->block_size);

        buffer_offset += block->block_size;

        free(file_content);
        fclose(current_file);
        free(complete_path);
    }

    // Write the first 4 bytes 
    printf("----------------------------------\n");

    printf("File Count %d\n", file_count);
    fwrite((char*)&file_count, sizeof(int), 1, fp);
    fwrite((char*)files, sizeof(file_block), file_count, fp);
    fwrite(file_buffer, buffer_size, 1, fp);
    printf("Buffer Size %d\n", (int)buffer_size);

    printf("----------------------------------\n");
    printf("Successfully Created Ramdisk.\n");
    printf("Size of Ramdisk %d.\n", (int)ftell(fp));
    printf("----------------------------------\n");

    free(file_buffer);
    fclose(fp);

    return 0;
}

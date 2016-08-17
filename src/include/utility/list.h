#ifndef __LIST_H__
#define __LIST_H__

typedef struct 
{
    void* value;
    struct list_node* next;
} list_node;

typedef struct
{
    struct list_node* next;
} list;

list* create_list();
void delete_list(list* node);
void list_push(list* node, void* value);
void list_remove(list* node, void* value);
unsigned long list_count(list* node);

#endif //__LIST_H__

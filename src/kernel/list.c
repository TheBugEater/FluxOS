#include <utility/list.h>
#include <utility/utility.h>

list* create_list()
{
    list* new_list = kmalloc(sizeof(list));
    memset(new_list, 0, sizeof(list));
    
    return new_list;
}

void delete_list(list* node)
{
    if(node != NULL)
    {
        kfree(node);
    }
}

void list_push(list* node, void* value)
{
    if(node != NULL)
    {
        list_node* new_node = kmalloc(sizeof(list_node));
        memset(new_node, 0, sizeof(new_node));
        new_node->value = value;
        new_node->next = NULL;

        list_node* last_node = node->next;
        if(last_node == NULL)
        {
            node->next = new_node;
            return;
        }

        while(last_node->next != NULL)
        {
            last_node = last_node->next;
        }
   
        last_node->next = new_node;
    }
}

void list_remove(list* node, void* value)
{
    if(node != NULL && node->next != NULL)
    {
        list_node* current_node = node->next;
        if(current_node->value == value)
        {
            kfree(current_node);
            node->next = NULL;
            return;
        }

        while(current_node->next != NULL)
        {
            list_node* next_node = current_node->next;
            if(next_node->value == value)
            {
                current_node->next = next_node->next;

                printk("Removed\n");

                kfree(next_node);
            }

            current_node = current_node->next;
        }
    }
}

unsigned long list_count(list* node)
{
    if(node == NULL)
        return 0;

    unsigned int val = 0;
    list_node* current_node = node->next;
    while(current_node)
    {
        current_node = current_node->next;
        val++;
    }

    return val;
}



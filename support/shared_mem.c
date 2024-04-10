#include "shared_mem.h"

struct mem_line {
    int id;
    int lock;
    size_t size;
    void* pointer;
};

static struct mem_line id_addr_table[SHARED_MEM_MAX_OBJ] = {0};
static int count = 0;

#define UNLOCK 0
#define LOCKED 1

int shared_mem_create(int id, size_t size)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].pointer == NULL)
        {
            id_addr_table[i].id = id;
            id_addr_table[i].size = size;

            id_addr_table[i].lock = LOCKED;
            id_addr_table[i].pointer = malloc(size);
            memset(id_addr_table[i].pointer, 0, size);
            id_addr_table[i].lock = UNLOCK;
            
            return 0;
        }
    }
    return -1;
}

int shared_mem_update(int id, const void* pointer)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].id == id)
        {
            if (id_addr_table[i].lock == UNLOCK)
            {
                id_addr_table[i].lock = LOCKED;
                memcpy(id_addr_table[i].pointer, pointer, id_addr_table[i].size);
                id_addr_table[i].lock = UNLOCK;
                return 0;
            }
            break;
        }
    }
    return -1;
}

void* shared_mem_get(int id)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].id == id && id_addr_table[i].size != 0)
        {
            if (id_addr_table[i].lock == UNLOCK)
                return id_addr_table[i].pointer;
            break;
        }
    }
    return NULL;
}

int shared_mem_delete(int id)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].id == id)
        {
            memset(&id_addr_table[i], 0, sizeof(struct mem_line));
            return 0;
        }
    }
    return 1;
}
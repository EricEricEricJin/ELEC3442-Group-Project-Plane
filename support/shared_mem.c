#include "board.h"
#include "shared_mem.h"
#include "pthread.h"

struct mem_line
{
    int id;
    pthread_mutex_t lock;
    size_t size;
    void *pointer;
};

static struct mem_line id_addr_table[SHARED_MEM_MAX_OBJ] = {0};
static int count = 0;

#define WAIT_US 100

int shared_mem_create(int id, size_t size)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].pointer == NULL)
        {
            // assign ID and size
            id_addr_table[i].id = id;
            id_addr_table[i].size = size;

            // Initialize lock
            pthread_mutex_init(&id_addr_table[i].lock, 0);

            // Allocate and clear memory
            pthread_mutex_lock(&id_addr_table[i].lock);
            id_addr_table[i].pointer = malloc(size);
            memset(id_addr_table[i].pointer, 0, size);
            pthread_mutex_unlock(&id_addr_table[i].lock);

            return 0;
        }
    }
    return -1;
}

int shared_mem_update(int id, const void *pointer)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].id == id)
        {
            pthread_mutex_lock(&id_addr_table[i].lock);
            memcpy(id_addr_table[i].pointer, pointer, id_addr_table[i].size);
            pthread_mutex_unlock(&id_addr_table[i].lock);
            return 0;
        }
    }
    return -1;
}

int shared_mem_get(int id, void* dst)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].id == id)
        {
            pthread_mutex_lock(&id_addr_table[i].lock);
            memcpy(dst, id_addr_table[i].pointer, id_addr_table[i].size);
            pthread_mutex_unlock(&id_addr_table[i].lock);
            return 0;
        }
    }
    return -1;
}

int shared_mem_delete(int id)
{
    for (int i = 0; i < SHARED_MEM_MAX_OBJ; i++)
    {
        if (id_addr_table[i].id == id)
        {
            // Free memory
            free(id_addr_table[i].pointer);
            // Destroy mutex lock
            pthread_mutex_destroy(&id_addr_table[i].lock);
            memset(&id_addr_table[i], 0, sizeof(struct mem_line));
            return 0;
        }
    }
    return 1;
}

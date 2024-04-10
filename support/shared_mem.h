#ifndef __SHARED_MEM_H__
#define __SHARED_MEM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHARED_MEM_MAX_OBJ 8

int shared_mem_create(int id, int size);
int shared_mem_update(int id, const void* pointer);
void* shared_mem_get(int id);
int shared_mem_delete(int id);

#endif
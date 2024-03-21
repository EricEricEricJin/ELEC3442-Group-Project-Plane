#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum device_type
{
    DEVICE_ESC,
    DEVICE_SERVO,
    DEVICE_SENSOR,
    DEVICE_UNKNOW
};

struct device
{
    char name[50];
    uint8_t type;
    void *param;
    void *user_data;

    void (*device_init)(void *param);
};

typedef struct device *device_t;

int32_t device_init(struct device *object, const char *name);
device_t device_find(const char *name, uint8_t type);
void device_detach(device_t object);


#endif
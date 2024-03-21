#include "device.h"

static device_t device_array[32] = {NULL};

char *device_name[50] =
    {
        "ESC",
        "SERVO",
        "SENSOR",
        "UNKNOWN",
};

int32_t device_init(device_t object, const char *name)
{
    // Assign name
    strcpy(object->name, name);

    // Add to list
    for (int i = 0; i < 32; i++)
    {
        if (device_array[i] == NULL)
        {
            device_array[i] = object;
            break;
        }
    }

    // print log, replace with log() later
    printf("%s register successful, type: %s.", name, device_name[object->type]);
}

device_t device_find(const char *name, uint8_t type)
{
    
}
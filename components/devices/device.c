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
    // REPLACE WITH DEVICE_ASSERT !!!
    if (object == NULL)
        return -1;
    
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
    if ((name == NULL) || (type >= DEVICE_UNKNOW))
    {
        return NULL;
    }
    device_t object;
    for (int i = 0; i < 32; i++)
    {
        object = device_array[i];
        if (object == NULL)
            break;
        if ((strncmp(object->name, name, strlen(name)) == 0) && (type == object->type))
            return object;
    }
    return NULL;
}

void device_detach(device_t object)
{
    // todo: check null

    for (int i = 0; i < 32; i++)
    {
        if (device_array[i] == object)
        {
            // delete object
            free(object);
            device_array[i] = NULL; // remove 
            
            // element move forward
            for (int j = i + 1; j < 32; j++)
            {
                device_array[j-1] = device_array[j];
                if (device_array[j] == NULL)
                    break;
            }
            
            break;
        }
    }
}
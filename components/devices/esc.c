#include "esc.h"

static fn_pwm_send esc_i2c_send = NULL;

int32_t esc_i2c_send_register(fn_pwm_send fn)
{
    if (fn == NULL)
        return -1;
    esc_i2c_send = fn;
    return 0;
}

int32_t esc_register(esc_device_t esc_dev, const char* name)
{
    if (esc_dev == NULL)
        return -1;
    
    
}
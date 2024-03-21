#include "esc.h"

static fn_pwm_send esc_pwm_send = NULL;

int32_t esc_i2c_send_register(fn_pwm_send fn)
{
    if (fn == NULL)
        return -1;
    esc_pwm_send = fn;
    return 0;
}

int32_t esc_register(esc_device_t esc_dev, const char *name)
{
    if (esc_dev == NULL)
        return -1;

    esc_dev->parent.type = DEVICE_ESC;

    // TODO: get data (current) ?? call back ??

    device_init(&(esc_dev->parent), name);

    // add to esc list
    for (int i = 0; i < 4; i++)
        if (esc_list[i] == NULL)
            esc_list[i] = esc_dev;

    return 0;
}

esc_data_t esc_get_data(esc_device_t esc_dev)
{
    // TODO: CHECK NULL
    return &(esc_dev->data);
}

int32_t esc_set_percent(esc_device_t esc_dev, int percent)
{
    // check input
    if (percent < 0 || percent > 100) // now no reverse.
        return -1;

    esc_dev->percentage = percent;
    return 0;
}

int32_t esc_pwm_output()
{
    for (int i = 0; i < 4; i++)
    {
        if (esc_list[i] != NULL)
        {
            auto object = esc_list[i];
            uint16_t pw = ((object->percentage) / 100.0f) * \
                (object->pw_full - object->pw_zero) + object->pw_zero;
            esc_pwm_send(esc_list[i]->channel, pw);
        }
    }
}

int32_t esc_data_update()
{
}
#ifndef __ESC_H__
#define __ESC_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "board.h"

typedef struct esc_data *esc_data_t;
typedef struct esc_device *esc_device_t;

typedef int32_t (*fn_pwm_send)(pca9685_t, int, uint16_t);

struct esc_data
{
    uint16_t current_raw;
    float current_amp
};


struct esc_device
{
    struct esc_data data;
    uint8_t channel;
    uint16_t pw_zero; // us
    uint16_t pw_full; // us
    float percentage;

    void (*get_data)(esc_device_t, uint8_t*);
};
int32_t esc_i2c_send_register(fn_pwm_send fn);
int32_t esc_register(esc_device_t esc_dev, const char* name);
esc_data_t esc_get_data(esc_device_t esc_dev);
int32_t esc_set_percent(esc_device_t esc_dev, float percent);

#endif
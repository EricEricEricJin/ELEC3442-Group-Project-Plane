#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "board.h"

typedef struct esc *esc_t;

struct esc
{
    pca9685_t pwm_dev;
    int pwm_channel;

    ads7830_t adc_dev;
    int adc_channel;
    float adc_gain;

    uint32_t zero_pw_us;
    uint32_t full_pw_us;
    uint32_t rev_pw_us; // set to 0 for no-rev

    float thrust;   // percentage 
    float current;
};

int esc_init(esc_t comp,
             pca9685_t pwm_dev, int pwm_channel,
             ads7830_t adc_dev, int adc_channel, float adc_gain,
             uint32_t zero_pw_us, uint32_t full_pw_us, uint32_t rev_pw_us);

int esc_start(esc_t comp);
int esc_stop(esc_t comp);
int esc_set_thrust(esc_t comp, float thrust);

float esc_get_current(esc_t comp);
float esc_get_thrust(esc_t comp);

#endif
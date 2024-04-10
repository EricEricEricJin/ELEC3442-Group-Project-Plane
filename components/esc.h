#ifndef __ESC_H__
#define __ESC_H__

#include "board.h"

typedef struct esc *esc_t;

typedef enum {
    ESC_STOPPED,
    ESC_RUNNING
} esc_state_t;

struct esc
{
    int pwm_channel;
    current_sensing_ch_t current_ch;

    uint32_t zero_pw_us;
    uint32_t full_pw_us;

    esc_state_t state;
    float thrust;    // use full scale of uint16
};

int esc_init(esc_t comp, int pwm_channel, current_sensing_ch_t current_ch, uint32_t zero_pw_us, uint32_t full_pw_us);

int esc_start(esc_t comp);
int esc_stop(esc_t comp);
int esc_set_thrust(esc_t comp, float thrust);

float esc_get_current(esc_t comp);
int esc_get_state(esc_t comp);
float esc_get_thrust(esc_t comp);

#endif
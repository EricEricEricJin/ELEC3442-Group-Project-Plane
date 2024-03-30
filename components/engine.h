#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "board.h"

typedef struct engine *engine_t;

struct engine
{
    int pwm_channel;
    current_sensing_ch_t current_ch;

    uint32_t zero_pw_us;
    uint32_t full_pw_us;

    uint16_t thrust;    // use full scale of uint16
};

int engine_init(engine_t comp, int pwm_channel, current_sensing_ch_t current_ch, uint32_t zero_pw_us, uint32_t full_pw_us);

int engine_start(engine_t comp);
int engine_stop(engine_t comp);
int engine_set_thrust(engine_t comp, uint16_t thrust);

float engine_get_current(engine_t comp);
uint16_t engine_get_thrust(engine_t comp);

#endif
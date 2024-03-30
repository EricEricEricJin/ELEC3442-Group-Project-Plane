#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "board.h"

typedef struct engine *engine_t;

typedef enum {
    ENGINE_STOPPED,
    ENGINE_RUNNING
} engine_state_t;

struct engine
{
    int pwm_channel;
    current_sensing_ch_t current_ch;

    uint32_t zero_pw_us;
    uint32_t full_pw_us;

    engine_state_t state;
    float thrust;    // use full scale of uint16
};

int engine_init(engine_t comp, int pwm_channel, current_sensing_ch_t current_ch, uint32_t zero_pw_us, uint32_t full_pw_us);

int engine_start(engine_t comp);
int engine_stop(engine_t comp);
int engine_set_thrust(engine_t comp, float thrust);

float engine_get_current(engine_t comp);
int engine_get_state(engine_t comp);
float engine_get_thrust(engine_t comp);

#endif
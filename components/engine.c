#include "engine.h"

int engine_init(engine_t comp, int pwm_channel, current_sensing_ch_t current_ch,
                uint32_t zero_pw_us, uint32_t full_pw_us)
{
    if (comp == NULL || full_pw_us <= zero_pw_us)
        return -1;

    memset(comp, 0, sizeof(struct engine));

    comp->pwm_channel = pwm_channel;
    comp->current_ch = current_ch;
    comp->zero_pw_us = zero_pw_us;
    comp->full_pw_us = full_pw_us;

    // reset engine output
    if (board_pwm_set_pw(pwm_channel, 0) != 0)
        return -1;

    return 0;
}

int engine_start(engine_t comp) { return board_pwm_set_pw(comp->pwm_channel, comp->zero_pw_us); }
int engine_stop(engine_t comp) { return board_pwm_set_pw(comp->pwm_channel, 0); }

int esc_set_thrust(engine_t comp, uint16_t thrust)
{
    // thrust = thrust > 1 ? 1 : (thrust < -1 ? -1 : thrust);

    // uint32_t pw = comp->zero_pw_us;
    // if (thrust >= 0) // forward
    //     pw += thrust * (comp->full_pw_us - comp->zero_pw_us);
    // else if (comp->rev_pw_us > 0) // reverse and support reverse
    //     pw += thrust * (comp->zero_pw_us - comp->rev_pw_us);
    // else // reverse but not support reverse, just zero.
    //     thrust = 0;
    uint32_t pw = ((float)thrust / UINT16_MAX) * (comp->full_pw_us - comp->zero_pw_us) + comp->zero_pw_us;
    if (board_pwm_set_pw(comp->pwm_channel, pw) != E_OK)
        return -1;
    comp->thrust = thrust;
    return 0;
}

uint16_t esc_get_thrust(engine_t comp) { return comp->thrust; }
float esc_get_current(engine_t comp) { return board_adc_get_current(comp->current_ch); }


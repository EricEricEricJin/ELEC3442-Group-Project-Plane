#include "esc.h"

int esc_init(esc_t comp, int pwm_channel, current_sensing_ch_t current_ch,
                uint32_t zero_pw_us, uint32_t full_pw_us)
{
    if (comp == NULL || PWM_INVALID_CH(pwm_channel) || full_pw_us <= zero_pw_us)
        return -1;

    memset(comp, 0, sizeof(struct esc));

    comp->pwm_channel = pwm_channel;
    comp->current_ch = current_ch;
    comp->zero_pw_us = zero_pw_us;
    comp->full_pw_us = full_pw_us;

    // reset esc output
    board_pwm_set_pw(pwm_channel, 0);
    comp->state = ESC_STOPPED;
    return 0;
}

int esc_start(esc_t comp) {
    if (comp->state == ESC_RUNNING || comp->thrust != 0)
        return -1;
    if (board_pwm_set_pw(comp->pwm_channel, comp->zero_pw_us) != E_OK)
        return -1;
    comp->state = ESC_RUNNING;
    return 0;
}

int esc_stop(esc_t comp) {
    if (comp->state == ESC_STOPPED)
        return -1;
    if (board_pwm_set_pw(comp->pwm_channel, 0) != E_OK)
        return -1;
    comp->thrust = 0;
    comp->state = ESC_STOPPED;
    return 0;
}

int esc_set_thrust(esc_t comp, float thrust)
{
    // thrust = thrust > 1 ? 1 : (thrust < -1 ? -1 : thrust);

    // uint32_t pw = comp->zero_pw_us;
    // if (thrust >= 0) // forward
    //     pw += thrust * (comp->full_pw_us - comp->zero_pw_us);
    // else if (comp->rev_pw_us > 0) // reverse and support reverse
    //     pw += thrust * (comp->zero_pw_us - comp->rev_pw_us);
    // else // reverse but not support reverse, just zero.
    //     thrust = 0;

    thrust = thrust > 1 ? 1 : (thrust < 0  ? 0 : thrust);
    if (comp->state != ESC_RUNNING)
        return -1;

    uint32_t pw = thrust * (comp->full_pw_us - comp->zero_pw_us) + comp->zero_pw_us;
    if (board_pwm_set_pw(comp->pwm_channel, pw) != E_OK)
        return -1;
    comp->thrust = thrust;
    return 0;
}

float esc_get_current(esc_t comp) { return board_adc_get_current(comp->current_ch); }
int esc_get_state(esc_t comp) {return comp->state; }
float esc_get_thrust(esc_t comp) { return comp->thrust; }

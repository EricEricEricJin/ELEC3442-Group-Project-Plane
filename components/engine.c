#include "engine.h"

int esc_init(esc_t comp,
             pca9685_t pwm_dev, int pwm_channel,
             ads7830_t adc_dev, int adc_channel, float adc_gain,
             uint32_t zero_pw_us, uint32_t full_pw_us, uint32_t rev_pw_us)
{
    if (comp == NULL || full_pw_us <= zero_pw_us || rev_pw_us >= zero_pw_us)
        return -1;

    memset(comp, 0, sizeof(struct esc));

    comp->pwm_dev = pwm_dev;
    comp->pwm_channel = pwm_channel;

    comp->zero_pw_us = zero_pw_us;
    comp->full_pw_us = full_pw_us;
    comp->rev_pw_us = rev_pw_us;

    comp->adc_dev = adc_dev;
    comp->adc_channel = adc_channel;
    comp->adc_gain = adc_gain;

    // reset engine output
    if (pca9685_set_pw(pwm_dev, pwm_channel, 0) != 0)
        return -1;

    return 0;
}

int esc_start(esc_t comp)
{
    return pca9685_set_pw(comp->pwm_dev, comp->pwm_channel, comp->zero_pw_us);
}

int esc_stop(esc_t comp)
{
    return pca9685_set_pw(comp->pwm_dev, comp->pwm_channel, 0);
}

int esc_set_thrust(esc_t comp, float thrust)
{
    thrust = thrust > 1 ? 1 : (thrust < -1 ? -1 : thrust);

    uint32_t pw = comp->zero_pw_us;
    if (thrust >= 0) // forward
        pw += thrust * (comp->full_pw_us - comp->zero_pw_us);
    else if (comp->rev_pw_us > 0) // reverse and support reverse
        pw += thrust * (comp->zero_pw_us - comp->rev_pw_us);
    else // reverse but not support reverse, just zero.
        thrust = 0;

    comp->thrust = thrust;
    return pca9685_set_pw(comp->pwm_dev, comp->pwm_channel, pw);
}

float esc_get_current(esc_t comp)
{
    // todo
}

float esc_get_thrust(esc_t comp) { return comp->thrust; }

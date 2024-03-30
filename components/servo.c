#include "servo.h"

uint32_t servo_deg_to_pw(servo_type_t type, float deg)
{
    switch (type)
    {
    case SERVO_180DEG:
        deg = deg > 90 ? 90 : (deg < -90 ? -90 : deg);
        return 1400 + 1000 * (deg / 90); // 400us to 2400us
    }
    return 0;
}

int servo_init(servo_t comp, servo_type_t type, int pwm_channel, int zero_deg, int max_deg, int min_deg)
{
    if (comp == NULL || PWM_INVALID_CH(pwm_channel))
        return -1;
    if (SERVO_INVALID_DEG(zero_deg, max_deg, min_deg))
        return -1;

    memset(comp, 0, sizeof(struct servo));

    comp->pwm_channel = pwm_channel;
    comp->type = type;

    comp->zero_deg = zero_deg;
    comp->min_deg = min_deg;
    comp->max_deg = max_deg;

    board_pwm_set_pw(pwm_channel, 0);
    comp->state = SERVO_OFF;

    return 0;
}

int servo_turn_on(servo_t comp)
{
    if (comp->state == SERVO_ON)
        return -1;
    if (board_pwm_set_pw(comp->pwm_channel, servo_deg_to_pwm(comp->type, comp->zero_deg)) != E_OK)
        return -1;
    comp->state = SERVO_ON;
    comp->degree = comp->zero_deg;
    return 0;
}

int servo_turn_off(servo_t comp)
{
    if (comp->state == SERVO_OFF)
        return -1;
    if (board_pwm_set_pw(comp->pwm_channel, 0) != E_OK)
        return -1;
    comp->state = SERVO_OFF;
    return 0;
}

int servo_set_deg_abs(servo_t comp, float deg)
{
    if (comp->state != SERVO_ON)
        return -1;
    
    if (board_pwm_set_pw(comp->pwm_channel, servo_deg_to_pw(comp->type, deg)) != E_OK)
        return -1;
    
    comp->degree = deg;
    return 0;
}

int servo_set_deg_trimmed(servo_t comp, float deg)
{
    if (comp->state != SERVO_ON)
        return -1;
    
    float deg_abs = comp->zero_deg + (comp->max_deg > comp->min_deg ? deg : -deg);
    if (SERVO_INVALID_DEG(deg_abs, comp->max_deg, comp->min_deg))
        return -1;
    if (board_pwm_set_pw(comp->pwm_channel, servo_deg_to_pw(comp->type, deg_abs)) != E_OK)
        return -1;
    comp->degree = deg_abs;
    return 0;
}
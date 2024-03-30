#ifndef __SERVO_H__
#define __SERVO_H__

#include "board.h"

typedef struct servo* servo_t;

#define SERVO_INVALID_DEG(deg, lima, limb) ((lima) >= (limb) ? \
    ((deg) > (lima) || (deg) < (limb)) : \
    ((deg) < (lima) || (deg) > (limb)))

typedef enum {
    SERVO_180DEG,
    SERVO_90DEG
} servo_type_t;

typedef enum {
    SERVO_ON,
    SERVO_OFF,
} servo_state_t;

struct servo {
    int pwm_channel;
    servo_type_t type;

    // 180 deg servo range is -90 to 90
    // 90 deg servo range is -45 to 45
    float zero_deg, max_deg, min_deg;

    servo_state_t state;
    float degree;
};

int servo_init(servo_t comp, servo_type_t type, int pwm_channel, int zero_deg, int max_deg, int min_deg);
int servo_turn_on(servo_t comp);
int servo_turn_off(servo_t comp);
int servo_set_deg_abs(servo_t comp, float deg);
int servo_set_deg_trimmed(servo_t comp, float deg);



#endif
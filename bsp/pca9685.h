/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#ifndef __PCA9685_H__
#define __PCA9685_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#define PWM_INVALID_CH(ch) ((ch) < 0 || (ch) > 15)

typedef struct pca9685 *pca9685_t;

struct pca9685
{
    uint16_t i2c_addr;
    int fd;
    uint32_t period_us;
    uint16_t channel_values[16];
};

int pca9685_init(pca9685_t dev, uint16_t i2c_addr, float frequency);
int pca9685_set_value(pca9685_t dev, int channel, uint16_t value); // not occupying can bus!
int pca9685_set_pw(pca9685_t dev, int channel, uint32_t pw_us);
int pca9685_send(pca9685_t dev); // using bus. called regularly in board.c

#endif

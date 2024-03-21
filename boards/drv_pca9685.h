#ifndef __PCA9685_H__
#define __PCA9685_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

typedef struct pca9685 *pca9685_t;

struct pca9685
{
    uint16_t i2c_addr;
    int fd;
    uint16_t frequency;
    uint16_t channel_values[16];
};

int32_t pca9685_init(pca9685_t dev, uint16_t i2c_addr, uint16_t frequency);
int32_t pca9685_set_channel(pca9685_t dev, int channel, uint16_t value); // not occupying can bus!
int32_t pca9685_send(pca9685_t dev);     // using bus. called regularly in board.c

#endif

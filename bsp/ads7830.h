#ifndef __ADS7830_H__
#define __ADS7830_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#define ADC_INVALID_CH(ch) ((ch) < 0 || (ch) > 7)

typedef struct ads7830 *ads7830_t;

struct ads7830
{
    uint16_t i2c_addr;
    int fd;
    float vref;
    uint8_t channel_values[8];
};

int ads7830_init(ads7830_t dev, uint16_t i2c_addr, float vref);
uint8_t ads7830_get_value(ads7830_t dev, int channel);
float ads7830_get_volt(ads7830_t dev, int channel);
int ads7830_update(ads7830_t dev);

#endif
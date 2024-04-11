#ifndef __BMP280_H__
#define __BMP280_H__

#include <wiringPi.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct bmp280* bmp280_t;

struct bmp280 {
    uint16_t i2c_addr;
    int fd;
    int value;
};

int bmp280_init(bmp280_t dev, uint16_t i2c_addr);

#endif
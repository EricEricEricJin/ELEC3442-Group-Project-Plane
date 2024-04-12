#include "jy901.h"
#include <string.h>
#include <stdio.h>

#include <wiringPiI2C.h>

float to_float(int x)
{
    float ret = x / 32768.0f;
    return ret >= 1 ? ret - 2 : ret;
}

#define WT16(fd, reg, value) wiringPiI2CWriteReg16((fd), (reg), (value))
#define RD16(fd, reg, value) wiringPiI2CReadReg16((fd), (reg))

int jy901_init(jy901_t dev, uint16_t i2c_addr, uint16_t orient, uint16_t axis6)
{
    if (dev == NULL)
        return -1;
    memset(dev, 0, sizeof(struct jy901));

    dev->i2c_addr = i2c_addr;
    dev->fd = wiringPiI2CSetup(dev->i2c_addr);
    if (dev->fd < 0)
        return -1;
    return jy901_restart(dev);
}

int jy901_restart(jy901_t dev) { return wiringPiI2CWriteReg16(dev->fd, 0x00, 0xFF); }

int jy901_update(jy901_t dev)
{
    dev->raw_data.a_x = wiringPiI2CReadReg16(dev->fd, 0x34);
    dev->raw_data.a_y = wiringPiI2CReadReg16(dev->fd, 0x35);
    dev->raw_data.a_z = wiringPiI2CReadReg16(dev->fd, 0x36);

    dev->raw_data.w_x = wiringPiI2CReadReg16(dev->fd, 0x37);
    dev->raw_data.w_y = wiringPiI2CReadReg16(dev->fd, 0x38);
    dev->raw_data.w_z = wiringPiI2CReadReg16(dev->fd, 0x39);

    dev->raw_data.roll = wiringPiI2CReadReg16(dev->fd, 0x3d);
    dev->raw_data.pitch = wiringPiI2CReadReg16(dev->fd, 0x3e);
    dev->raw_data.yaw = wiringPiI2CReadReg16(dev->fd, 0x3f);
}

int jy901_decode(jy901_data_raw_t raw, jy901_data_decoded_t decoded)
{
    decoded->a_x = to_float(raw->a_x);
    decoded->a_y = to_float(raw->a_y);
    decoded->a_z = to_float(raw->a_z);

    decoded->w_x = to_float(raw->w_x);
    decoded->w_y = to_float(raw->w_y);
    decoded->w_z = to_float(raw->w_z);

    decoded->b_x = to_float(raw->b_x);
    decoded->b_y = to_float(raw->b_y);
    decoded->b_z = to_float(raw->b_z);

    decoded->roll = to_float(raw->roll);
    decoded->pitch = to_float(raw->pitch);
    decoded->yaw = to_float(raw->yaw);

    return 0;
}
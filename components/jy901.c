#include "jy901.h"

#include <wiringPiI2C.h>

float read_value(int fd, int reg, int lim);

int jy901_init(jy901_t dev, uint16_t i2c_addr, uint16_t orient, uint16_t axis6)
{
    if (dev == NULL)
        return -1;
    memset(dev, 0, sizeof(struct jy901));

    dev->i2c_addr = i2c_addr;
    dev->fd = wiringPiI2CSetup(dev->i2c_addr);
    if (dev->fd < 0)
        return -1;
    return 0;
}

int jy901_cal(jy901_t dev)
{
}

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

float read_value(int fd, int reg, int lim)
{
    int raw = wiringPiI2CReadReg16(fd, reg);
    float ret = raw / 32768.0f * lim;
    if (ret >= lim)
        ret -= 2 * lim;
    return ret;
}
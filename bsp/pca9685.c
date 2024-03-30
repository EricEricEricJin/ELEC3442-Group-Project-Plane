#include "pca9685.h"

#define INVALID_DEV(dev) ((dev) == NULL || (dev)->fd < 0)

const float freq_fix_coef = 1.071;

int pca9685_init(pca9685_t dev, uint16_t i2c_addr, float frequency)
{
    if (dev == NULL)
        return -1;
    memset(dev, 0, sizeof(struct pca9685));

    dev->i2c_addr = i2c_addr;

    // software reset
    int reset_fd = wiringPiI2CSetup(0x0);
    wiringPiI2CWrite(reset_fd, 0x06);

    // check parameters
    dev->fd = wiringPiI2CSetup(dev->i2c_addr);
    if (dev->fd < 0)
        return -1;

    // set frequency
    uint8_t prescale = 25000000 / (4096 * frequency) - 1;
    dev->period_us = 1000000 / (25000000.0f / (4096 * (prescale + 1)) * freq_fix_coef);

    if (prescale < 0x03)
        return -1; // 8-bit, will not larger than 0xff.

    uint8_t mode1;
    // enable auto increment and enter sleep
    mode1 = wiringPiI2CReadReg8(dev->fd, 0x00);
    mode1 &= 0x7f;
    mode1 |= (1 << 5);
    mode1 |= (1 << 4);
    wiringPiI2CWriteReg8(dev->fd, 0x00, mode1);

    // set frequency
    wiringPiI2CWriteReg8(dev->fd, 0xFE, prescale);

    mode1 = wiringPiI2CReadReg8(dev->fd, 0x00);

    // exit sleep
    mode1 &= 0xEF;
    wiringPiI2CWriteReg8(dev->fd, 0x00, mode1);

    delay(1); // wait for > 500us

    // restart
    mode1 |= (1 << 7);
    wiringPiI2CWriteReg8(dev->fd, 0x00, mode1);

    // clear all outputs
    for (int i = 0; i < 16; i++)
    {
        wiringPiI2CWriteReg16(dev->fd, 0x06 + (i << 2), 0);
        wiringPiI2CWriteReg16(dev->fd, 0x06 + (i << 2) + 2, 0);
    }

    return 0;
}

int pca9685_set_value(pca9685_t dev, int channel, uint16_t value)
{
    if (INVALID_DEV(dev) || PWM_INVALID_CH(channel))
        return -1;
    if (value > 0xfff)
        return -1;
    dev->channel_values[channel] = value;
    return 0;
}

int pca9685_set_pw(pca9685_t dev, int channel, uint32_t pw_us)
{
    if (INVALID_DEV(dev) || PWM_INVALID_CH(channel))
        return -1;

    // check value
    if (pw_us > dev->period_us)
        return -1;

    // compute and assign value
    dev->channel_values[channel] = 0xfff * ((float)pw_us / (float)(dev->period_us));
    return 0;
}

int pca9685_send(pca9685_t dev)
{
    for (int i = 0; i < 16; i++)
    {
        // wiringPiI2CWriteReg16(dev->fd, 0x06 + (i << 2), 0); already set to 0 in init
        wiringPiI2CWriteReg16(dev->fd, 0x06 + (i << 2) + 2, dev->channel_values[i] & 0xfff);
    }
    return 0;
}

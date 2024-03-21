#include "drv_pca9685.h"

const float freq_fix_coef = 1.071;

int32_t pca9685_init(pca9685_t dev, uint16_t i2c_addr, uint16_t frequency)
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
    dev->frequency = 25000000 / (4096 * (prescale + 1)) * freq_fix_coef;
    printf("real f = %d\n", dev->frequency);
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

    delay(1);   // wait for > 500us

    // restart
    mode1 |= (1 << 7);
    wiringPiI2CWriteReg8(dev->fd, 0x00, mode1);

    return 0;
}

int32_t pca9685_set_channel(pca9685_t dev, int channel, uint16_t value)
{
    if (dev->fd == -1 || channel > 15)
        return -1;

    dev->channel_values[channel] = value;
    return 0;
}

int32_t pca9685_send(pca9685_t dev)
{
    for (int i = 0; i < 16; i++)
    {
        wiringPiI2CWriteReg16(dev->fd, 0x06 + (i << 2), 0);
        wiringPiI2CWriteReg16(dev->fd, 0x06 + (i << 2) + 2, dev->channel_values[i] & 0xfff);
    }
    return 0;
}


#include "ads7830.h"
#include <ads1115.h>

#define INVALID_DEV(dev) ((dev) == NULL || (dev)->fd < 0)

#define CH_SEL(x) ((x)==0 ? 0b000 : \
                   (x)==1 ? 0b100 : \
                   (x)==2 ? 0b001 : \
                   (x)==3 ? 0b101 : \
                   (x)==4 ? 0b010 : \
                   (x)==5 ? 0b110 : \
                   (x)==6 ? 0b011 : \
                   (x)==7 ? 0b111 : 0)


int ads7830_init(ads7830_t dev, uint16_t i2c_addr, float vref)
{
    if (dev == NULL)
        return -1;
    memset(dev, 0, sizeof(struct ads7830));

    dev->i2c_addr = i2c_addr;

    // check parameters
    dev->fd = wiringPiI2CSetup(dev->i2c_addr);
    if (dev->fd < 0)
        return -1;

    // commands
    // for (int i = 0; i < 8; i++)
    // {
    //     wiringPiI2CWrite(dev->fd, 0b10000100 | (i << 4));
    // }

    return 0;
}

uint8_t ads7830_get_value(ads7830_t dev, int channel)
{
    if (INVALID_DEV(dev) || ADC_INVALID_CH(channel))
        return 0;
    return dev->channel_values[channel];
}

float ads7830_get_volt(ads7830_t dev, int channel)
{
    if (INVALID_DEV(dev) || ADC_INVALID_CH(channel))
        return 0;
    return dev->vref * ((float)(dev->channel_values[channel]) / 255.0f);
}

int ads7830_update(ads7830_t dev)
{
    for (int i = 0; i < 8; i++)
    {
        wiringPiI2CWrite(dev->fd, 0b10000100 | (CH_SEL(i) << 4));
        printf("sel = %x, ", CH_SEL(i));
        dev->channel_values[i] = wiringPiI2CRead(dev->fd);
        delay(1);
    }
    return 0;
}
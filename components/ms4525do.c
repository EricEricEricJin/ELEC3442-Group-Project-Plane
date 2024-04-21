#include "ms4525do.h"

#include <wiringPi.h>
#include <math.h>
#include <unistd.h>
#include "board.h"

int ms4525do_init(ms4525do_t dev, uint16_t i2c_addr)
{
    if (dev == NULL)
        return -1;
    memset(dev, 0, sizeof(struct ms4525do));

    dev->i2c_addr = i2c_addr;
    dev->fd = wiringPiI2CSetup(dev->i2c_addr);
    if (dev->fd < 0)
        return -1;

    return 0;
}

int ms4525do_update(ms4525do_t dev)
{
    uint16_t p_dat, t_dat;
    // fetch raw
    uint8_t raw_buf[4];
    read(dev->fd, raw_buf, 4);

    p_dat = ((raw_buf[0] & 0x3F) << 8) | raw_buf[1];
    t_dat = (raw_buf[2] << 3) | (raw_buf[3] >> 5);

    dev->data.p_psi = fabs((p_dat - 819.15f) / 14744.7f - 0.49060678f);
    dev->data.v_meterps = sqrt(dev->data.p_psi * 13789.5144f / 1.225f);
    dev->data.t_degc = t_dat * 0.09770395701f - 50;
    return 0;
}

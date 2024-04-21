#ifndef __MS4525DO_H__
#define __MS4525DO_H__

#include <stdint.h>

typedef struct ms4525do* ms4525do_t;

struct ms4525do_data {
    float p_psi;
    float v_meterps;
    float t_degc;
};

struct ms4525do
{
    uint16_t i2c_addr;
    int fd;
    struct ms4525do_data data;
};

int ms4525do_init(ms4525do_t dev, uint16_t i2c_addr);
int ms4525do_update(ms4525do_t dev);


#endif
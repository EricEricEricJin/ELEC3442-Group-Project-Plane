#ifndef __JY901_H__
#define __JY901_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

enum {
    JY901_ORIENT_H,
    JY901_ORIENT_V,
};

enum {
    JY901_AXIS_6,
    JY901_AXIS_9,
};

typedef struct jy901* jy901_t;

// Currently the basic JY901
// Update to JY901B later to access GPS and Barometer (?)
struct jy901_data_raw
{
    uint16_t a_x, a_y, a_z;
    uint16_t w_x, w_y, w_z;
    uint16_t b_x, b_y, b_z;
    uint16_t roll, pitch, yaw;
};

struct jy901
{
    uint16_t i2c_addr;
    int fd;

    struct jy901_data_raw raw_data;
    pthread_mutex_t lock;

};

int jy901_init(jy901_t dev, uint16_t i2c_addr, uint16_t orient, uint16_t axis6);
int jy901_cal(jy901_t dev);
int jy901_update(jy901_t dev);

#endif
#ifndef __PLANE_DATA_H__
#define __PLANE_DATA_H__

#include <stdint.h>

typedef struct plane_data* plane_data_t;

struct plane_data
{
    int16_t accel_x, accel_y, accel_z;
    int16_t omega_x, omega_y, omega_z;
    int16_t mag_x, mag_y, mag_z;
    int16_t angle_x, angle_y, angle_z;

    uint16_t tof;
    uint16_t air_spd;

    uint8_t volt_main, volt_bus, volt_aux;
    uint16_t altitude;
    uint16_t crc16;
};




#endif
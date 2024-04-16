/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#ifndef __PLANE_DATA_H__
#define __PLANE_DATA_H__

#include <stdint.h>

typedef struct plane_data *plane_data_t;

#define DATA_MAX_DELAY_MS (100)
struct __attribute__((__packed__)) plane_data
{
    int16_t a_x, a_y, a_z;
    int16_t w_x, w_y, w_z;
    int16_t b_x, b_y, b_z;
    int16_t roll, pitch, yaw;

    uint16_t tof;
    uint16_t air_spd;

    uint8_t volt_bat, volt_bus, volt_aux;
    uint16_t altitude, temperature;
    uint32_t update_time_ms;
    uint16_t crc16;
};

#endif
/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#include "sensor_task.h"
#include "board.h"

int *imu_init()
{
}

int *imu_routine()
{
}

int *bmp280_init()
{
}

int *bmp280_routine()
{
}

int *speedometer_init()
{
}

int *speedometer_routine()
{
}

void sensor_init()
{
    // register sensors
    board_i2c_routine_register(imu_init, imu_routine, 20);
    board_i2c_routine_register(bmp280_init, bmp280_routine, 50);
    board_i2c_routine_register(speedometer_init, speedometer_routine, 50);
}

void sensor_task(void const *argument)
{
    while (1)
    {
        board_delay_ms(5);
    }
}
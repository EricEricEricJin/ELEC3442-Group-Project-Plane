/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#include "sensor_task.h"
#include "board.h"

#include "jy901.h"
#include "shared_mem.h"
#include "plane_data.h"
#include "msg_id.h"

struct jy901 imu;

int imu_init()
{
    return jy901_init(&imu, 0x50, 0, 0);
}

int imu_routine()
{
    jy901_update(&imu);
}

int bmp_init()
{
}

int bmp_routine()
{
}

int speedometer_init()
{
}

int speedometer_routine()
{
}

void sensor_init()
{
    // register sensors
    board_i2c_routine_register((i2c_init_func_t)&imu_init, (i2c_routine_func_t)&imu_routine, 20);
    board_i2c_routine_register((i2c_init_func_t)&bmp_init, (i2c_routine_func_t)&bmp_routine, 50);
    board_i2c_routine_register((i2c_init_func_t)&speedometer_init, (i2c_routine_func_t)&speedometer_routine, 50);
}

void sensor_task(void const *argument)
{
    struct plane_data data;
    while (1)
    {
        // Update board data
        data.volt_bat = board_adc_get_vbat() / 12.9f * 255.0f;
        data.volt_bus = board_adc_get_vbus() / 5.5f * 255.0f;
        data.volt_aux = board_adc_get_vaux() / 4.3f * 255.0f;
        data.update_time_ms = board_get_millis();

        // Update IMU data
        data.a_x = imu.raw_data.a_x;
        data.a_y = imu.raw_data.a_y;
        data.a_z = imu.raw_data.a_z;

        data.w_x = imu.raw_data.w_x;
        data.w_y = imu.raw_data.w_y;
        data.w_z = imu.raw_data.w_z;

        data.b_x = imu.raw_data.b_x;
        data.b_y = imu.raw_data.b_y;
        data.b_z = imu.raw_data.b_z;

        data.roll = imu.raw_data.roll;
        data.pitch = imu.raw_data.pitch;
        data.yaw = imu.raw_data.yaw;

        // Update BMP data, todo

        shared_mem_update(DATA_MSG_ID, &data);
        board_delay_ms(10);
    }
}
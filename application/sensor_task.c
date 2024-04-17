/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#include <math.h>

#include "sensor_task.h"
#include "board.h"

#include "shared_mem.h"
#include "msg_id.h"

#include "plane_data.h"
#include "ground_cmd.h"

#include "jy901.h"
#include "bmp280.h"

#define JY901_ADDR (0x50)
#define BMP280_ADDR (0x76)

struct jy901 imu;
struct bmp280 bmp;

int imu_init()
{
    return jy901_init(&imu, JY901_ADDR, 0, 0);
}

int imu_routine()
{
    return jy901_update(&imu);
}

int bmp_init()
{
    return bmp280_init(&bmp, BMP280_ADDR);
}

int bmp_routine()
{
    return bmp280_update(&bmp);
}

int speedometer_init()
{
    return 0;
}

int speedometer_routine()
{
    return 0;
}

float bar_altitude_meter(float seaLevelhPa, float pressure)
{
    // https://github.com/adafruit/Adafruit_BMP280_Library/blob/master/Adafruit_BMP280.cpp
    return 44330 * (1.0 - pow(pressure / seaLevelhPa, 0.1903));
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
    struct ground_cmd cmd;
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

        printf("sensor task: %x %x\n", data.roll, data.pitch);

        // Update BMP data
        shared_mem_get(CMD_MSG_ID, &cmd);
        data.altitude = bar_altitude_meter(cmd.QNH_mPa / 1000.0f, bmp.data.pressure);
        data.temperature = bmp.data.temperature;

        shared_mem_update(DATA_MSG_ID, &data);
        board_delay_ms(10);
    }
}
// This file is modified based on BMP280/BMP280.c
// by leon-anavi@github
// URL: https://github.com/leon-anavi/rpi-examples/blob/master/BMP280/c/BMP280.c

#include "board.h"
#include "bmp280.h"

/* BMP280 default address */
#define BMP280_I2CADDR 0x76
#define BMP280_CHIPID 0xD0

/* BMP280 Registers */
#define BMP280_DIG_T1 0x88 /* R   Unsigned Calibration data (16 bits) */
#define BMP280_DIG_T2 0x8A /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_T3 0x8C /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P1 0x8E /* R   Unsigned Calibration data (16 bits) */
#define BMP280_DIG_P2 0x90 /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P3 0x92 /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P4 0x94 /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P5 0x96 /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P6 0x98 /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P7 0x9A /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P8 0x9C /* R   Signed Calibration data (16 bits) */
#define BMP280_DIG_P9 0x9E /* R   Signed Calibration data (16 bits) */

#define BMP280_CONTROL 0xF4 /*sets the data acquisition options of the device*/
#define BMP280_RESET 0xE0
#define BMP280_CONFIG 0xF5 /*sets the rate, filter and interface options of the device*/
#define BMP280_PRESSUREDATA 0xF7
#define BMP280_TEMPDATA 0xFA

uint32_t read_20bit(int fd, int msb_reg)
{
    return ((wiringPiI2CReadReg8(fd, msb_reg) << 16) |
            (wiringPiI2CReadReg8(fd, msb_reg + 1) << 8) |
            (wiringPiI2CReadReg8(fd, msb_reg + 2))) >> 4;
}

void bmp280_cal(bmp280_t dev)
{
    dev->cal.cal_t1 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_T1);
    dev->cal.cal_t2 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_T2);
    dev->cal.cal_t3 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_T3);
    dev->cal.cal_p1 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_P1);
    dev->cal.cal_p2 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_P2);
    dev->cal.cal_p3 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_P3);
    dev->cal.cal_p4 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_P4);
    dev->cal.cal_p5 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_P5);
    dev->cal.cal_p6 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_P6);
    dev->cal.cal_p7 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_P7);
    dev->cal.cal_p8 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_P8);
    dev->cal.cal_p9 = wiringPiI2CReadReg16(dev->fd, BMP280_DIG_P9);
}

int32_t compensate_temp(int raw_temp, struct bmp280_cal* cal)
{
    // Magic stuff by leon..
    int t1 = (((raw_temp >> 3) - (cal->cal_t1 << 1)) * (cal->cal_t2)) >> 11;
    int t2 = (((((raw_temp >> 4) - (cal->cal_t1)) *
                ((raw_temp >> 4) - (cal->cal_t1))) >>
               12) *
              (cal->cal_t3)) >>
             14;
    return t1 + t2;
}

int bmp280_init(bmp280_t dev, uint16_t i2c_addr)
{
    if (dev == NULL)
        return -1;
    memset(dev, 0, sizeof(struct bmp280));

    dev->i2c_addr = i2c_addr;
    dev->fd = wiringPiI2CSetup(dev->i2c_addr);
    if (dev->fd < 0)
        return -1;

    bmp280_cal(dev);
    wiringPiI2CWriteReg8(dev->fd, BMP280_CONTROL, 0b11110011);
    return 0;
}

int bmp280_update(bmp280_t dev)
{
    // Temperature
    int raw_temp = read_20bit(dev->fd, BMP280_TEMPDATA);
    int comp_temp = compensate_temp(raw_temp, &dev->cal);
    dev->data.temperature = ((comp_temp * 5 + 128) >> 8) / 100.0f;

    // Pressure
    int raw_press = read_20bit(dev->fd, BMP280_PRESSUREDATA);

    // Magic thing on pressure ... 
    int64_t p1 = comp_temp / 2 - 64000;
    int64_t p2 = p1 * p1 * (int64_t)dev->cal.cal_p6 / 32768;
    int64_t buf = (p1 * (int64_t)dev->cal.cal_p5 * 2);
    p2 += buf << 17;
    p2 += (int64_t)dev->cal.cal_p4 << 35;
    p1 = ((p1 * p1 * dev->cal.cal_p3) >> 8) + ((p1 * dev->cal.cal_p2) << 12);
    p1 = (((int64_t)1 << 47) + p1) * ((int64_t)dev->cal.cal_p1) >> 33;

    // Avoid exception caused by division by zero
    if (0 == p1)
    {
        // printf("bmp div by 0 err!\n");
        dev->data.pressure = 0.0f;
    }
    else 
    {
        int64_t p = 1048576 - raw_press;
        p = (((p << 31) - p2) * 3125) / p1;
        p1 = ((int64_t)dev->cal.cal_p9 * (p >> 13) * (p >> 13)) >> 25;
        p2 = ((int64_t)dev->cal.cal_p8 * p) >> 19;
        p = ((p + p1 + p2) >> 8) + (((int64_t)dev->cal.cal_p7) << 4);
        dev->data.pressure = p / 256.0f;
    }
    // printf("BMP update! %f\n", dev->data.pressure);
    return 0;
}


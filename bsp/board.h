#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#include "pca9685.h"
#include "ads7830.h"

#define E_OK 0

#define MAX_I2C_ROUTINE 8
typedef enum board_state {
    BOARD_RUNNING, 
    BOARD_INITED,
} board_state_t;

typedef enum current_sensing_ch {
    CURRENT_CH1 = 1,
    CURRENT_CH2 = 2,
} current_sensing_ch_t;

typedef int (*i2c_init_func)();
typedef int (*i2c_routine_func)();
typedef struct board *board_t;

struct board
{
    struct pca9685 dev_pca9685;
    struct ads7830 dev_ads7830;
    
    i2c_init_func i2c_init_func_list[MAX_I2C_ROUTINE];
    i2c_routine_func i2c_routine_func_list[MAX_I2C_ROUTINE];
    
    unsigned int i2c_period_list[MAX_I2C_ROUTINE]; // ms
    unsigned int i2c_update_time_list[MAX_I2C_ROUTINE];
    
    pthread_t thread_id;
    int i2c_routine_cnt;
    board_state_t state;
};

/*
calling sequence:
    init() --> register & set pwm freq --> start()
*/

int board_init();
int board_start();
void board_stop();

int board_i2c_routine_register(i2c_init_func init_func, i2c_routine_func routine_func, unsigned int period_ms);
board_t get_board();

void board_pwm_set_freq(float freq_hz);
int board_pwm_set_value(int channel, uint16_t value);
int board_pwm_set_pw(int channel, uint32_t pw_us);

float board_adc_get_vbus();
float board_adc_get_vbat();
float board_adc_get_vaux();
float board_adc_get_current(current_sensing_ch_t current_ch); // not ADC ch!


#endif
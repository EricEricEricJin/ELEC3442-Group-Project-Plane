/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#include <wiringPi.h>
#include "board.h"

#define PCA9685_ADDR (0x40)
#define ADS7830_ADDR (0x48)

#define ADC_VREF (3.3f)

#define ADC_VBAT_CH (0)
#define ADC_VBAT_GAIN (6.10)

#define ADC_I_GAIN (100.0)

#define ADC_VBUS_CH (3)
#define ADC_VBUS_GAIN (1.75)

#define ADC_VAUX_CH (4)
#define ADC_VAUX_GAIN (1.51)

float pwm_frequency;
static struct board theboard;

int bd_pca9685_init() { return pca9685_init(&(theboard.dev_pca9685), PCA9685_ADDR, pwm_frequency); }
int bd_pca9685_routine() { return pca9685_send(&(theboard.dev_pca9685)); }
int bd_ads7830_init() { return ads7830_init(&(theboard.dev_ads7830), ADS7830_ADDR, ADC_VREF); }
int bd_ads7830_routine() { return ads7830_update(&(theboard.dev_ads7830)); }

void *board_loop();

int board_init()
{
    memset(&theboard, 0, sizeof(struct board));

    // register on-board chips
    board_i2c_routine_register(&bd_pca9685_init, &bd_pca9685_routine, 5);
    board_i2c_routine_register(&bd_ads7830_init, &bd_ads7830_routine, 50);

    return E_OK;
}

int board_start()
{
    // I2C Initialize
    for (int i = 0; i < theboard.i2c_routine_cnt; i++)
        if (theboard.i2c_init_func_t_list[i]() != E_OK)
            return -1;

    theboard.state = BOARD_RUNNING;
    // create thread
    return pthread_create(&theboard.thread_id, NULL, board_loop, NULL);
}

void board_stop()
{
    theboard.state = BOARD_INITED;
    pthread_join(theboard.thread_id, NULL);
}

int board_i2c_routine_register(i2c_init_func_t init_func, i2c_routine_func_t routine_func, unsigned int period_ms)
{
    if (theboard.i2c_routine_cnt >= MAX_I2C_ROUTINE)
        return -1;
    theboard.i2c_init_func_t_list[theboard.i2c_routine_cnt] = init_func;
    theboard.i2c_routine_func_t_list[theboard.i2c_routine_cnt] = routine_func;
    theboard.i2c_period_list[theboard.i2c_routine_cnt] = period_ms;
    theboard.i2c_routine_cnt++;
    return 0;
}

void *board_loop()
{
    unsigned int current_time;
    while (theboard.state == BOARD_RUNNING)
    {
        current_time = millis();
        int i;
        for (i = 0; i < theboard.i2c_routine_cnt; i++)
        {
            if (current_time - theboard.i2c_update_time_list[i] >= theboard.i2c_period_list[i])
            {
                theboard.i2c_routine_func_t_list[i]();
                theboard.i2c_update_time_list[i] = current_time;
            }
        }
        if (i == 0)
            delay(1);
    }
}

board_t get_board() { return &theboard; }

// PWM functions
void board_pwm_set_freq(float freq_hz) { pwm_frequency = freq_hz; }
int board_pwm_set_value(int channel, uint16_t value) { return pca9685_set_value(&theboard.dev_pca9685, channel, value); }
int board_pwm_set_pw(int channel, uint32_t pw_us) 
{
    // printf("Board set pw!\n"); 
    return pca9685_set_pw(&theboard.dev_pca9685, channel, pw_us); 
}

// ADC functions
float board_adc_get_vbus() { return ads7830_get_volt(&theboard.dev_ads7830, ADC_VBUS_CH) * ADC_VBUS_GAIN; }
float board_adc_get_vbat() { return ads7830_get_volt(&theboard.dev_ads7830, ADC_VBAT_CH) * ADC_VBAT_GAIN; }
float board_adc_get_vaux() { return ads7830_get_volt(&theboard.dev_ads7830, ADC_VAUX_CH) * ADC_VAUX_GAIN; }
float board_adc_get_ch_volt(int ch) { return ads7830_get_volt(&theboard.dev_ads7830, ch); }
// uint8_t board_adc_get_ch_raw(int ch) { return ads7830_get_value(&theboard.dev_ads7830, ch); }

float board_get_cpu_temp()
{
    return pisystem_get_cpu_temp();
}

unsigned int board_get_millis()
{
    return millis();
}

void board_delay_ms(unsigned int ms) { delay(ms); }
void board_delay_us(unsigned int us) { delayMicroseconds(us); }
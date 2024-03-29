#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "pca9685.h"
#include "ads7830.h"

#define E_OK 0

#define MAX_I2C_ROUTINE 8
enum {
    BOARD_RUNNING, 
    BOARD_INITED,
};

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
    int state;
};

static struct board theboard;

int board_init();
int board_start();
void board_stop();

int board_i2c_routine_register(i2c_init_func init_func, i2c_routine_func routine_func, unsigned int period_ms);
board_t get_board();

#endif
#include "board.h"

int bd_pca9685_init() { return pca9685_init(&(theboard.dev_pca9685), 0x40, 200); }
int bd_pca9685_routine() { return pca9685_send(&(theboard.dev_pca9685)); }
int bd_ads7830_init() { return ads7830_init(&(theboard.dev_ads7830), 0x48, 3.3); }
int bd_ads7830_routine() { return ads7830_update(&(theboard.dev_ads7830)); }

void *board_loop();

int board_init()
{
    memset(&theboard, 0, sizeof(struct board));

    // register on-board chips
    board_i2c_routine_register(bd_pca9685_init, bd_pca9685_routine, 5);
    board_i2c_routine_register(bd_ads7830_init, bd_ads7830_routine, 50);

    return E_OK;
}

int board_start()
{
    // I2C Initialize
    for (int i = 0; i < theboard.i2c_routine_cnt; i++)
        if (theboard.i2c_init_func_list[i]() != E_OK)
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

int board_i2c_routine_register(i2c_init_func init_func, i2c_routine_func routine_func, unsigned int period_ms)
{
    if (theboard.i2c_routine_cnt >= MAX_I2C_ROUTINE)
        return -1;
    theboard.i2c_init_func_list[theboard.i2c_routine_cnt] = init_func;
    theboard.i2c_routine_func_list[theboard.i2c_routine_cnt] = routine_func;
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
                theboard.i2c_routine_func_list[i]();
                theboard.i2c_update_time_list[i] = current_time;
            }
        }
        if (i == 0)
            delay(1);
        
    }
}

board_t get_board() {return &theboard;}
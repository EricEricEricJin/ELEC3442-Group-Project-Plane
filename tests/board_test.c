#include <stdio.h>
#include "board.h"


int main()
{
    board_init();
    board_start();
    delay(1000);
    printf("set ret = %d\n", pca9685_set_pw(&(get_board()->dev_pca9685), 0, 400));
    delay(1000);
    for (int i = 0; i < 8; i++)
    {
        uint8_t value = ads7830_get_value(&(get_board()->dev_ads7830), i);
        float v = ads7830_get_volt(&(get_board()->dev_ads7830), i);
        printf("v[%d] = %x:%f\n",i, value, v);
    }
    printf("vref=%f\n", get_board()->dev_ads7830.vref);
    board_stop();

    return 0;
}
#ifndef __GROUND_CMD_H__
#define __GROUND_CMD_H__

#include <time.h>
#include <stdint.h>

typedef struct ground_cmd* ground_cmd_t;

struct ground_cmd
{
    int16_t elevator;
    int16_t aileron;
    int16_t rudder;
    
    int16_t throttle_1;
    int16_t throttle_2;

    uint8_t op_mode;
    
    time_t update_time;
    uint16_t crc16;
};




#endif 
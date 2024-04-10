/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

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

    uint8_t eng_1;
    uint8_t eng_2;
    uint16_t thrust_1;
    uint16_t thrust_2;

    uint8_t op_mode;
    int16_t trim_elevator;
    int16_t trim_aileron;
    
    uint32_t update_time_ms;
    uint16_t crc16;
};




#endif 
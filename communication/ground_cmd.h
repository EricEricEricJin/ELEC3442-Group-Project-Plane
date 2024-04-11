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

typedef struct ground_cmd *ground_cmd_t;

#define CMD_MAX_DELAY_MS (500)

enum
{
    OPMODE_MANUAL = 0,
    OPMODE_STABILIZE = 1,
    OPMODE_LOCK_ATT = 2,
};

struct __attribute__((__packed__)) ground_cmd
{
    int eng_1 : 1;
    int eng_2 : 1;
    int opmode_elevator : 2;
    int opmode_aileron : 2;
    int opmode_rudder : 2;

    int16_t elevator;
    int16_t aileron;
    int16_t rudder;

    uint16_t thrust_1;
    uint16_t thrust_2;

    int16_t trim_elevator;
    int16_t trim_aileron;
    int16_t trim_rudder;

    uint32_t update_time_ms;
    uint16_t crc16;
};

#endif
/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#ifndef __CTRL_SURFACE_H__
#define __CTRL_SURFACE_H__

#include "servo.h"
#include "pid.h"
#include "servo.h"

typedef struct ctrl_surface *ctrl_surface_t;

enum
{
    CTRL_SURFACE_MODE_OFF,
    CTRL_SURFACE_MODE_STABILIZE,
    CTRL_SURFACE_MODE_LOCK_ATT,
};

struct ctrl_surface
{
    int mode;

    struct pid inter_pid, outer_pid;

    float input;    // is sidestick input for STA or OFF, and is target angle for ATT
    float pos_feedback, spd_feedback;
};

void ctrl_surface_init(ctrl_surface_t ctrl_surface, struct pid_param inter_param, struct pid_param outer_param);

float ctrl_surface_calculate(ctrl_surface_t ctrl_surface);

int ctrl_surface_set_mode(ctrl_surface_t ctrl_surface, int mode);
int ctrl_surface_set_feedback(ctrl_surface_t ctrl_surface, float pos_feedback, float spd_feedback);
int ctrl_surface_set_input(ctrl_surface_t ctrl_surface, float input);

#endif
/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#include "ctrl_surface.h"
#include "communication.h"

void ctrl_surface_init(ctrl_surface_t ctrl_surface, struct pid_param pid_param)
{
    ctrl_surface->mode = OPMODE_MANUAL;
    pid_struct_init(&ctrl_surface->pid, pid_param.max_out, pid_param.integral_limit, pid_param.p, pid_param.i, pid_param.d);
    ctrl_surface->input = 0.0f;
    ctrl_surface->pos_feedback = 0.0f;
    ctrl_surface->spd_feedback = 0.0f;
}

float ctrl_surface_calculate(ctrl_surface_t ctrl_surface)
{
    float output = 0;

    switch (ctrl_surface->mode)
    {
    case OPMODE_STABILIZE:
        break;

    case OPMODE_LOCK_ATT:
        break;

    default: // OFF
        output = ctrl_surface->input;
        break;
    }

    printf("ctrl_surface calculate");

    return output;
}

int ctrl_surface_set_mode(ctrl_surface_t ctrl_surface, int mode)
{
    ctrl_surface->mode = mode;
}

int ctrl_surface_set_feedback(ctrl_surface_t ctrl_surface, float pos_feedback, float spd_feedback)
{
    ctrl_surface->pos_feedback = pos_feedback;
    ctrl_surface->spd_feedback = spd_feedback;
}

int ctrl_surface_set_input(ctrl_surface_t ctrl_surface, float input)
{
    ctrl_surface->input = input;
}
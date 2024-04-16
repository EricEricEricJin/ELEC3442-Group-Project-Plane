/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#include "ctrl_surface.h"
#include "communication.h"

void ctrl_surface_init(ctrl_surface_t ctrl_surface, struct pid_param inter_param, struct pid_param outer_param)
{
    ctrl_surface->mode = OPMODE_MANUAL;
    pid_struct_init(&ctrl_surface->outer_pid, outer_param.max_out, outer_param.integral_limit, outer_param.p, outer_param.i, outer_param.d);
    pid_struct_init(&ctrl_surface->inter_pid, inter_param.max_out, inter_param.integral_limit, inter_param.p, inter_param.i, inter_param.d);
    ctrl_surface->input = 0.0f;
    ctrl_surface->pos_feedback = 0.0f;
    ctrl_surface->spd_feedback = 0.0f;
}

float ctrl_surface_calculate(ctrl_surface_t ctrl_surface)
{
    float servo_out = 0;
    float outer_out = ctrl_surface->input;
    
    switch (ctrl_surface->mode)
    {
    case OPMODE_LOCK_ATT:
        outer_out = pid_calculate(&ctrl_surface->outer_pid, ctrl_surface->pos_feedback, ctrl_surface->input);
    case OPMODE_STABILIZE:
        servo_out = pid_calculate(&ctrl_surface->inter_pid, ctrl_surface->spd_feedback, ctrl_surface->input);
        break;
    default: // OFF
        servo_out = ctrl_surface->input;
        break;
    }

    // printf("ctrl_surface calculate");
    return servo_out;
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
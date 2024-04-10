#ifndef __CTRL_SURFACE_H__
#define __CTRL_SURFACE_H__

#include "servo.h"
#include "pid.h"
#include "servo.h"

typedef struct ctrl_surface* ctrl_surface_t;

enum {
    CTRL_SURFACE_MODE_OFF,
    CTRL_SURFACE_MODE_STABILIZE,
    CTRL_SURFACE_MODE_LOCK_ATT,
};

struct ctrl_surface
{
    int mode;

    struct pid pid;

    float input;
    float pos_feedback, spd_feedback;
};


void ctrl_surface_init(ctrl_surface_t ctrl_surface, struct pid_param pid_param);

float ctrl_surface_calculate(ctrl_surface_t ctrl_surface);

int ctrl_surface_set_mode(ctrl_surface_t ctrl_surface, int mode);
int ctrl_surface_set_feedback(ctrl_surface_t ctrl_surface, float pos_feedback, float spd_feedback);
int ctrl_surface_set_input(ctrl_surface_t ctrl_surface, float input);

#endif
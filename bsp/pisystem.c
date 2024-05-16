/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#include <wiringPi.h>
#include "pisystem.h"

float pisystem_get_cpu_temp()
{
    int n;
    FILE *f_cputemp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (f_cputemp == NULL) return 0.0;
    fscanf(f_cputemp, "%d", &n);
    return n / 1000.0f;
    // return 0.0;
}

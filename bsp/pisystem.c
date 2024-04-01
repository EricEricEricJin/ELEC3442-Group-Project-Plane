#include <wiringPi.h>
#include "pisystem.h"

float pisystem_get_cpu_temp()
{
    int n;
    FILE* f_cputemp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    fscanf(f_cputemp, "%d", &n);
    return n / 1000.0f;
}

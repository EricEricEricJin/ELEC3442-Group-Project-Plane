#include <stdio.h>
#include <unistd.h>
#include "ms4525do.h"

int main()
{
    struct ms4525do sensor;
    ms4525do_init(&sensor, 0x28);

    while (1)
    {
        ms4525do_update(&sensor);
        printf("%f\t%f\t%f\n", sensor.data.t_degc, sensor.data.p_psi, sensor.data.v_meterps);
        sleep(1);
    }

    return 0;
}
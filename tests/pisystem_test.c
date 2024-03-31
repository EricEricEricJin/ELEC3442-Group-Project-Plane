#include "stdio.h"
#include "pisystem.h"
int main()
{
    float t = pisystem_get_cpu_temp();
    printf("CPU temperature is %f degrees C\n", t);
    return 0;
}
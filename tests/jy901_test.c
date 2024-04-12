#include "jy901.h"
#include "unistd.h"

float two_byte_to_float(int x)
{
    float ret = x / 32768.0f;
    return ret >= 1 ? ret - 2 : ret;
}


int main()
{
    struct jy901 jy901;
    jy901_init(&jy901, 0x50, 0, 0);
    jy901_restart(&jy901);
    usleep(1000);
    jy901_calibrate(&jy901);
    usleep(1000);
    while (1)
    {
        jy901_update(&jy901);
        printf("%f\t%f\t%f\n", 180*two_byte_to_float(jy901.raw_data.roll), 180*two_byte_to_float(jy901.raw_data.pitch), 180*two_byte_to_float(jy901.raw_data.yaw));
        usleep(100000);
    }
    
}
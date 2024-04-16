#include "bmp280.h"
int main()
{
    struct bmp280 bmp280;
    bmp280_init(&bmp280, 0x76);
    
    delay(1000);
    bmp280_update(&bmp280);
    float t = bmp280.data.temperature;
    float p = bmp280.data.pressure;
    printf("%f, %f\n", t, p);
    return 0;
}
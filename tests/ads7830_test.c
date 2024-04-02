#include "ads7830.h"

int main()
{
    struct ads7830 mydev;
    printf("init = %d\n", ads7830_init(&mydev, 0x48, 3.3));
    delay(10);
    float i1;
    float i2;
    
    while (1)
    {
        i1 = 0;
        i2 = 0;
        for(int i = 0; i < 16; i++)
        {
            ads7830_update(&mydev);
            i1 += ads7830_get_volt(&mydev, 1);
            i2 += ads7830_get_volt(&mydev, 2);
            delay(10);
        }
        i1 /= 16;
        i2 /= 16;
        printf("i1 = %f, i2 = %f\n", i1, i2);
        delay(500);
    }
    // for (int i = 0; i < 8; i++)
    //     printf("V[%d] = %d  ", i, ads7830_get_value(&mydev, i));
    printf("\n");
    return 0;
}
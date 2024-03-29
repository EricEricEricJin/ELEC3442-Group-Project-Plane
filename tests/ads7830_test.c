#include "ads7830.h"

int main()
{
    struct ads7830 mydev;
    printf("init = %d\n", ads7830_init(&mydev, 0x48, 3.3));
    delay(10);
    ads7830_update(&mydev);
    for (int i = 0; i < 8; i++)
        printf("V[%d] = %d  ", i, ads7830_get_value(&mydev, i));
    printf("\n");
    return 0;
}
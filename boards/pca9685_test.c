#include "drv_pca9685.h"

int main()
{
    struct pca9685 mydev;
    int32_t ret;
    ret = pca9685_init(&mydev, 0x40, 250);
    printf("init ret = %d\n", ret);

    // return 0;

    ret = pca9685_set_channel(&mydev, 0, 0x1ff);
    printf("channel ret = %d\n", ret);

    ret = pca9685_send(&mydev);
    printf("send ret = %d\n", ret);

    return 0;

}
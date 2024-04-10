#include "pca9685.h"

int main()
{
    struct pca9685 dev;
    pca9685_init(&dev, 0x40, 50);
    
    for (int i = 0; i < 14; i++)
        pca9685_set_pw(&dev, i, 900);
    
    pca9685_send(&dev);
    delay(2000);
    
    for (int i = 0; i < 14; i++)
        pca9685_set_pw(&dev, i, 1900);
    
    pca9685_send(&dev);
    delay(2000);

    for (int i = 0; i < 14; i++)
        pca9685_set_pw(&dev, i, (400+2400)/2);
    
    pca9685_send(&dev);

    return 0;

}

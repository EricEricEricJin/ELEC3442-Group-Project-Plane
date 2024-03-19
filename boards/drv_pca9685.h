
typedef struct pca9685 *pca9685_t;

struct pca9685
{
    uint16_t i2c_addr;
    uint16_t channel_values[16];
};


int32_t pca9685_init(pca);
int32_t pca9685_set_channel(channel, value); // not occupying can bus!
int32_t pca9685_send(void);     // called regularly in board.c
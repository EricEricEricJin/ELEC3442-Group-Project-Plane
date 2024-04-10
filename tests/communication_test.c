#include "communication.h"
#include <stdio.h>
#include <unistd.h>

const char* IP = "154.221.20.43";
const uint16_t PORT = 1234;

int main()
{
    struct plane_data data;
    data.angle_x = 567;

    struct ground_cmd cmd;
    struct communication comm;

    int ret;

    ret = communication_init(&comm, &cmd, &data, "192.168.141.105", 1234, 8888);
    printf("comm init = %d\n", ret);

    communication_set_send_rate(&comm, 500000);
    ret = communication_start(&comm);
    printf("comm start = %d\n", ret);

    for (int i = 0; i < 10; i++)
    {
        printf("state = %x, ", communication_get_state(&comm));
        printf("cmd = %x\n", cmd.elevator)
        sleep(1);
    }
    return 0;
}


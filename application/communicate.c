#include <unistd.h>
#include "communicate.h"
#include "communication.h"
#include "msg_id.h"
#include "shared_mem.h"
#include "plane_data.h"
#include "ground_cmd.h"

const char* SERVER_IP = "154.221.20.43";
const uint16_t SERVER_PORT = 1234;
const uint16_t MY_PORT = 1235;

struct communication ct_comm;
struct plane_data ct_data;
struct ground_cmd ct_cmd;

void communicate_task_init()
{
    int ret = communication_init(&ct_comm, &ct_cmd, &ct_data, MY_PORT, SERVER_IP, SERVER_PORT, 0xffff, 500000);
    printf("communicate task init = ret\n");
}

void communicate_send_task(void const *argument)
{
    printf("communicate send task start \n");
    while (1)
    {
        shared_mem_get(DATA_MSG_ID, ct_comm.data);
        
        // for test only
        ct_comm.data->accel_x = 1234;

        communication_send(&ct_comm);
        usleep(ct_comm.period_us);

        printf("communicate send task loop \n");
    }
}

void communicate_recv_task(void const *argument)
{       
    printf("communicate recv task start \n");
    while (1)
    {
        communication_recv(&ct_comm);
        shared_mem_update(CMD_MSG_ID, ct_comm.cmd);

        printf("communicate recv t1=%d t2=%d \n", ct_comm.cmd->thrust_1, ct_comm.cmd->thrust_2);
    }
}
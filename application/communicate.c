#include <unistd.h>
#include "communicate.h"
#include "communication.h"
#include "msg_id.h"
#include "shared_mem.h"
#include "plane_data.h"
#include "ground_cmd.h"

#include "appcfg.h"

struct communication ct_comm;
struct plane_data ct_data;
struct ground_cmd ct_cmd;
struct fdbk_data ct_fdbk;

void communicate_task_init()
{
    int ret = communication_init(&ct_comm, &ct_cmd, &ct_data, MY_PORT, SERVER_IP, SERVER_PORT, 0xffff, 100000);
    if (ret < 0)
    {
        printf("comm ret = %d\n", ret);
        exit(1);
    }
    printf("communicate task init = ret\n");
}

void communicate_send_task(void const *argument)
{
    printf("communicate send task start \n");
    while (1)
    {
        shared_mem_get(DATA_MSG_ID, &ct_data);
        shared_mem_get(FDBK_MSG_ID, &ct_fdbk);
        ct_data.elevator = ct_fdbk.elevator;
        ct_data.aileron_l = ct_fdbk.aileron_l;
        ct_data.aileron_r = ct_fdbk.aileron_r;
        ct_data.rudder = ct_fdbk.rudder;
        ct_data.eng_1 = ct_fdbk.eng_1;
        ct_data.eng_2 = ct_fdbk.eng_2;

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

int communicate_video_start()
{
    return communication_video_start(VIDEO_W, VIDEO_H, VIDEO_FR, VIDEO_BR, SERVER_IP, VIDEO_PORT);
}

int communicate_video_stop()
{
    return communication_video_stop();
}
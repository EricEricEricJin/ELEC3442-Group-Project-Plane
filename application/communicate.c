#include <unistd.h>
#include "communicate.h"
#include "communication.h"
#include "msg_id.h"
#include "shared_mem.h"
#include "plane_data.h"
#include "ground_cmd.h"

struct communication ct_comm;
struct plane_data ct_data;
struct ground_cmd ct_cmd;

void communicate_task_init()
{
    communication_init(&ct_comm, &ct_cmd, &ct_data, "127.0.0.1", 1234, 0xffff, 50000);
}

void communicate_send_task(void const *argument)
{
    while (1)
    {
        shared_mem_get(DATA_MSG_ID, ct_comm.data);
        communication_send(&ct_comm);
        usleep(ct_comm.period_us);
    }
}

void communicate_recv_task(void const *argument)
{
    while (1)
    {
        communication_recv(&ct_comm);
        shared_mem_update(CMD_MSG_ID, ct_comm.cmd);
    }
}
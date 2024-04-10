/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#include <pthread.h>

#include "init.h"
#include "plane_task.h"
#include "sensor_task.h"
#include "communication.h"
#include "shared_mem.h"
#include "msg_id.h"

#include "plane_data.h"
#include "ground_cmd.h"

pthread_t sensor_thread, plane_thread, comm_thread;

void task_init()
{
    // start communication

    // start plane task
    pthread_create(plane_thread, NULL, plane_task, NULL);
}

void sys_init()
{
    // create shared memories
    shared_mem_create(CMD_MSG_ID, sizeof(struct ground_cmd));
    shared_mem_create(DATA_MSG_ID, sizeof(struct plane_data));

    // sensor task init
    pthread_create(&sensor_thread, NULL, sensor_task, NULL);
    // todo: add parameter
}

void hw_init()
{
    board_init();
    sensor_init();
    board_start();
}
/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/
#include <stdio.h>
#include <pthread.h>

#include "init.h"
#include "board.h"
#include "plane_task.h"
#include "sensor_task.h"
#include "communication.h"
#include "shared_mem.h"
#include "msg_id.h"

#include "plane_data.h"
#include "ground_cmd.h"
#include "communicate.h"

pthread_t sensor_thread, plane_thread, comm_send_thread, comm_recv_thread;

void task_init()
{
    printf("Task init...\n");
    // start communication
    communicate_task_init();
    // communicate_video_start();
    pthread_create(&comm_send_thread, NULL, (void*(*)(void*))&communicate_send_task, NULL);
    pthread_create(&comm_recv_thread, NULL, (void*(*)(void*))&communicate_recv_task, NULL);

    // start plane task
    pthread_create(&plane_thread, NULL, (void*(*)(void*))&plane_task, NULL);
}

void sys_init()
{
    printf("Sys init...\n");
    // create shared memories
    shared_mem_create(CMD_MSG_ID, sizeof(struct ground_cmd));
    shared_mem_create(DATA_MSG_ID, sizeof(struct plane_data));

    // sensor task init
    pthread_create(&sensor_thread, NULL,  (void*(*)(void*))&sensor_task, NULL);
}

void hw_init()
{
    int ret;
    printf("HW init...\n");
    ret = board_init();
    printf("board init = %d\n", ret);
    board_pwm_set_freq(200);
    sensor_init();
    ret = board_start();
    printf("board start = %d\n", ret);
}
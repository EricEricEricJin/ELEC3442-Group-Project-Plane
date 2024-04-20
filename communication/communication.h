/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__
#include <netinet/in.h>
#include <pthread.h>

#include "plane_data.h"
#include "ground_cmd.h"

typedef struct communication *communication_t;

enum
{
    COMM_STATE_RUNNING = 1 << 0,
    COMM_RECV_ERROR = 1 << 1,
    COMM_SEND_ERROR = 1 << 2,
    COMM_RECV_TIMEOUT = 1 << 3,
};

struct communication
{
    int fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in my_addr;

    ground_cmd_t cmd;
    unsigned int last_update_time;

    plane_data_t data;
    unsigned int period_us;

    pthread_t tid_send;
    pthread_t tid_recv;
    pthread_mutex_t mutex_cmd;
    pthread_mutex_t mutex_data;

    uint16_t crc_value;
    int state;
};

int communication_init(communication_t comm, ground_cmd_t cmd, plane_data_t data,
                       uint16_t my_port, const char *server_ip, uint16_t server_port, uint16_t crc_value, unsigned int send_period_us);
int communication_send(communication_t comm);
int communication_recv(communication_t comm);

int communication_deinit(communication_t comm);

int communication_video_start(int width, int height, int framerate, int bitrate, const char* ip, int port);
int communication_video_stop();

uint16_t crc16(uint16_t crc, const void *buf, size_t size);

#endif
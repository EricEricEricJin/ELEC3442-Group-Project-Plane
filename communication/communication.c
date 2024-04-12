/******************************************************************************
 * Copyright (C) 2024 Eric Jin <https://github.com/ericericericjin>
 *
 * Everyone is permitted to copy and distribute verbatim or modified copies
 * of this program, and changing it is allowed as long as the name is changed.
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "communication.h"
#include "board.h"

// void comm_send_task(void const* argument)
// {
//     printf("entered send_thread\n");
//     communication_t comm = (communication_t)argument;

//     int ret;
//     while (comm->state | COMM_STATE_RUNNING)
//     {
//         // append checksum
//         comm->data->crc16 = crc16(comm->crc_value, (const void *)(comm->data), sizeof(struct plane_data) - sizeof(uint16_t));
//         // send to server
//         ret = sendto(comm->fd, (void *)(comm->data), sizeof(*comm->data), 0, (struct sockaddr *)(&comm->server_addr), sizeof(comm->server_addr));

//         // update state
//         if (ret < 0)
//             comm->state |= COMM_SEND_ERROR;
//         else
//             comm->state &= ~COMM_SEND_ERROR;

//         usleep(comm->period_us);
//     }
// }

int communication_send(communication_t comm)
{
    printf("fd = %d\n", comm->fd);
    // append checksum
    comm->data->crc16 = crc16(comm->crc_value, (const void *)(comm->data), sizeof(struct plane_data) - sizeof(uint16_t));
    printf("CRC = %x\n", comm->data->crc16);
    // send to server
    int ret = sendto(comm->fd, (void *)(comm->data), sizeof(*comm->data), 0, (struct sockaddr *)(&comm->server_addr), sizeof(comm->server_addr));
    printf("communication ret = %d \n", ret);
    // update state
    if (ret < 0)
        comm->state |= COMM_SEND_ERROR;
    else
        comm->state &= ~COMM_SEND_ERROR;

    return ret;
}

// void comm_recv_task(void const *argument)
// {
//     communication_t comm = (communication_t)argument;

//     int ret;
//     socklen_t seraddrlen = sizeof(comm->server_addr); // server address length

//     while (comm->state | COMM_STATE_RUNNING)
//     {
//         /*
//             MSG_WAITALL: block until specified length is received
//             MSG_WAITFORONE: block until at least 1 packet is available
//             MSG_DONTWAIT: nonblocking
//         */

//         // receive from server
//         ret = recvfrom(comm->fd, (void *)(comm->cmd), sizeof(*comm->cmd), MSG_WAITALL, (struct sockaddr *)&comm->server_addr, &seraddrlen);
//         if (ret != sizeof(comm->cmd))
//         {
//             // receive error
//             comm->state |= COMM_RECV_ERROR;
//         }
//         else
//         {
//             uint16_t crc_calc = crc16(comm->crc_value, (const void *)(comm->cmd), sizeof(struct ground_cmd) - sizeof(uint16_t));
//             if (crc_calc != comm->cmd->crc16)
//             {
//                 // received data incorrect
//                 comm->state |= COMM_RECV_ERROR;
//             }
//             else
//             {
//                 // received data correct
//                 memcpy(comm->cmd, comm->cmd, sizeof(struct ground_cmd));
//                 comm->last_update_time = board_get_millis();
//                 comm->state &= ~COMM_RECV_ERROR;
//             }
//         }
//     }
// }

int communication_recv(communication_t comm) // is blocking!
{
    // sleep(1);
    // return 0;
    int ret;
    socklen_t seraddrlen = sizeof(comm->server_addr); // server address length
    /*
        MSG_WAITALL: block until specified length is received
        MSG_WAITFORONE: block until at least 1 packet is available
        MSG_DONTWAIT: nonblocking
    */

    // receive from server
    ret = recvfrom(comm->fd, (void *)(comm->cmd), sizeof(*comm->cmd), MSG_WAITALL, (struct sockaddr *)&comm->server_addr, &seraddrlen);
    printf("communication ret = %d \n", ret);
    if (ret != sizeof(comm->cmd))
    {
        // receive error
        comm->state |= COMM_RECV_ERROR;
        return -1;
    }
    else
    {
        uint16_t crc_calc = crc16(comm->crc_value, (const void *)(comm->cmd), sizeof(struct ground_cmd) - sizeof(uint16_t));
        if (crc_calc != comm->cmd->crc16)
        {
            // received data incorrect
            comm->state |= COMM_RECV_ERROR;
            return -1;
        }
        else
        {
            // received data correct
            comm->state &= ~COMM_RECV_ERROR;
            return 0;
        }
    }
}

int communication_init(communication_t comm, ground_cmd_t cmd, plane_data_t data, uint16_t my_port,
                     const char* server_ip, uint16_t server_port, uint16_t crc_value, unsigned int send_period_us)
{
    // Initialize struct and socket
    memset(comm, 0, sizeof(struct communication));
    comm->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (comm->fd < 0)
        return -1;

    // Configure and Bind plane address
    memset(&comm->my_addr, 0, sizeof(comm->my_addr));
    comm->my_addr.sin_family = AF_INET;
    comm->my_addr.sin_port = htons(my_port);
    comm->my_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(comm->fd, (struct sockaddr *)&comm->my_addr, sizeof(comm->my_addr)) < 0)
        return -1;

    // Configure server address
    memset(&comm->server_addr, 0, sizeof(comm->server_addr));
    comm->server_addr.sin_family = AF_INET;
    comm->server_addr.sin_port = htons(server_port);
    comm->server_addr.sin_addr.s_addr = inet_addr(server_ip);

    comm->cmd = cmd;
    comm->data = data;
    comm->period_us = send_period_us;

    printf("communication inited!\n");
    comm->crc_value = crc_value;

    return 0;
}

// int communication_start(communication_t comm)
// {
//     int ret;
//     comm->state |= COMM_STATE_RUNNING;
//     ret |= pthread_create(&comm->tid_send, NULL, send_thread, (void *)comm);
//     ret |= pthread_create(&comm->tid_recv, NULL, recv_thread, (void *)comm);
//     return ret;
// }

// int communication_get_state(communication_t comm) { return comm->state; }

// int communication_stop(communication_t comm)
// {
//     comm->state &= ~COMM_STATE_RUNNING;
//     if (shutdown(comm->fd, SHUT_RDWR) == -1)
//         return -1;

//     pthread_join(comm->tid_send, NULL);
//     pthread_join(comm->tid_recv, NULL);

//     return 0;
// }

int communication_deinit(communication_t comm)
{
    if (shutdown(comm->fd, SHUT_RDWR) == -1)
        return -1;
    return 0;
}
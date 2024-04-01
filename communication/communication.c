#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "communication.h"
#include "board.h"

void *send_thread(void* arg)
{
    communication_t comm = (communication_t)arg;

    int ret;
    while (comm->state | COMM_STATE_RUNNING)
    {
        // append checksum
        comm->data->crc16 = crc16(comm->crc_value, (const void *)(comm->data), sizeof(struct plane_data) - sizeof(uint16_t));

        // send to server
        ret = sendto(comm->fd, (void *)(comm->data), sizeof(*comm->data), 0, &(comm->addr), sizeof(comm->addr));

        // update state
        if (ret < 0)
            comm->state |= COMM_SEND_ERROR;
        else
            comm->state &= ~COMM_SEND_ERROR;

        usleep(comm->period_us);
    }
}

void *recv_thread(void* arg)
{
    communication_t comm = (communication_t)arg;

    int ret;
    while (comm->state | COMM_STATE_RUNNING)
    {
        /*
            MSG_WAITALL: block until specified length is received
            MSG_WAITFORONE: block until at least 1 packet is available
            MSG_DONTWAIT: nonblocking
        */
        ret = recvfrom(comm->fd, (void *)(comm->cmd_nc), sizeof(*comm->cmd_nc), MSG_WAITALL, &comm->addr, sizeof(comm->addr));
        
        if (ret != sizeof(comm->cmd_nc))
        {
            // receive error
            comm->state |= COMM_RECV_ERROR;
        }
        else
        {
            uint16_t crc_calc = crc16(comm->crc_value, (const void *)(comm->cmd_nc), sizeof(struct ground_cmd) - sizeof(uint16_t));
            if (crc_calc != comm->cmd_nc->crc16)
            {
                // received data incorrect
                comm->state |= COMM_RECV_ERROR;
            }
            else
            {
                // received data correct
                memcpy(comm->cmd, comm->cmd_nc, sizeof(struct ground_cmd));
                comm->last_update_time = board_get_millis();
                comm->state &= ~COMM_RECV_ERROR;
            }
        }
    }
}

int communication_init(communication_t comm, ground_cmd_t cmd, plane_data_t data,
                       const char *IP, uint16_t port, uint16_t crc_value)
{
    memset(comm, 0, sizeof(struct communication));

    comm->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (comm->fd < 0)
        return -1;

    memset(&comm->addr, 0, sizeof(comm->addr));
    comm->addr.sin_family = AF_INET;
    comm->addr.sin_port = htons(port);
    comm->addr.sin_addr.s_addr = inet_addr(IP);

    comm->cmd_nc = malloc(sizeof(struct ground_cmd));

    return 0;
}

void communication_set_send_rate(communication_t comm, unsigned int send_period_us)
{
    comm->period_us = send_period_us;
}

int communication_start(communication_t comm)
{
    int ret;
    comm->state |= COMM_STATE_RUNNING;
    ret |= pthread_create(&comm->tid_send, NULL, send_thread, (void*)comm);
    ret |= pthread_create(&comm->tid_recv, NULL, recv_thread, (void*)comm);
    return ret;
}

int communication_stop(communication_t comm)
{
    comm->state &= ~COMM_STATE_RUNNING;
    // close(comm->fd); // close socket
    if (shutdown(comm->fd, SHUT_RDWR) == -1)
        return -1;

    pthread_join(comm->tid_send, NULL);
    pthread_join(comm->tid_recv, NULL); 
    
    return 0;
}

void communication_deinit(communication_t comm)
{
    free(comm->cmd_nc);
}
#include "communication.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
    struct plane_data data;
    data.gyro_x = 567;

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
        printf("state = %x ", comm.state);
        sleep(1);
    }
    return 0;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/socket.h>

// #include <netinet/in.h>
// #include <arpa/inet.h>

// int main()
// {
//     int fd;
//     fd = socket(AF_INET, SOCK_DGRAM, 0);
//     if (fd < 0)
//         return 1;
    
//     struct sockaddr_in server_addr;
//     memset(&server_addr, 0, sizeof(server_addr));

//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(1234);
//     server_addr.sin_addr.s_addr = inet_addr("192.168.31.230");


//     char buffer[1024] = {0};
//     strcpy(buffer, "Hi dude\n");

//     ssize_t sent_byte = sendto(fd, buffer, 1024, 0, (struct sockaddr*)(&server_addr), sizeof(server_addr));
//     printf("sent %ld\n", sent_byte);

//     return 0;

// }

// #include "comm_sender.h"

// const char buf[16] = "Hey dude";


// int main()
// {
//     int ret;

//     struct comm_sender mysender;
//     ret = comm_sender_init(&mysender, "192.168.31.230", 1234);
//     printf("init = %d\n", ret);

//     ret = comm_sender_send(&mysender, (const void*)buf, 6);
//     printf("send = %d\n", ret);

//     ret = comm_sender_close(&mysender);
//     printf("close = %d\n", ret);

//     return 0;

// }
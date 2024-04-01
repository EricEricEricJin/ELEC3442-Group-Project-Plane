#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int ret;
    int fd;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
        return 1;
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = inet_addr("192.168.141.105");

    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(1234);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(fd, (struct sockaddr*)&my_addr, sizeof(my_addr));
    printf("bind %d\n", ret);


    char buffer[1024] = {0};
    // ret = recv(fd, (void*)buffer, sizeof(buffer), MSG_WAITFORONE);
    int sal = sizeof(server_addr);
    ret = recvfrom(fd, (void*)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr*)&server_addr, &sal);
    
    printf("recv %d\n", ret);

    return 0;

}

/*
    summary:
    1. bind to local addr with INADDR_ANY
    2. recvfrom with server's addr
*/
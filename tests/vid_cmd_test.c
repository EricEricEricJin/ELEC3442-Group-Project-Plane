#include "stdio.h"
#include "stdlib.h"

int communication_video_start(int width, int height, int framerate, int bitrate, const char* ip, int port)
{
    char cmd[130];
    sprintf(cmd, "rpicam-vid -t 0 --width %d --height %d --framerate %d -b %d --inline -o udp://%s:%d > /dev/null 2>&1 &",
            width, height, framerate, bitrate, ip, port);
    printf("%s\n", cmd);
    return system(cmd);
}

int main()
{
    int ret = communication_video_start(640, 480, 10, 200000, "154.221.20.43", 1236);
    printf("ret = %d\n", ret);
    return 0;
}
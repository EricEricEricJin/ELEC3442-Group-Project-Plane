#ifndef __COMMUNICATE_H__
#define __COMMUNICATE_H__

void communicate_task_init();
void communicate_send_task(void const *argument);
void communicate_recv_task(void const *argument);

int communicate_video_start();
int communicate_video_stop();

#endif
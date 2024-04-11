#ifndef __COMMUNICATE_H__
#define __COMMUNICATE_H__

void communicate_task_init();
void communicate_send_task(void const *argument);
void communicate_recv_task(void const *argument);

#endif
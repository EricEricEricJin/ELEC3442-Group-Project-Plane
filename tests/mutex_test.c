#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;

int count;

void thread_run(void *arg)
{
    int i;
    pthread_mutex_lock(&mutex);
    for (i = 0; i < 3; i++)
    {
        printf("[%#lx]value of count: %d\n", pthread_self(), ++count);
        sleep(1);
    }
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
    pthread_t thread1;
    pthread_mutex_init(&mutex, 0);
    // pthread_create(&thread1, NULL, &thread_run, 0);
    pthread_create(&thread1, NULL, (void*(*)(void*))&thread_run, 0);
    
    printf("started\n");
    sleep(1);
    pthread_mutex_lock(&mutex);
    printf("locked in main\n");

    pthread_join(thread1, 0);
    pthread_mutex_destroy(&mutex);
    return 0;
}
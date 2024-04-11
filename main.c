#include "init.h"
#include <unistd.h>
int main()
{
    hw_init();
    sys_init();
    task_init();

    while (1)
    {
        sleep(1);
    }

    return 0;
}
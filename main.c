#include "init.h"

int main()
{
    hw_init();
    sys_init();
    task_init();
    return 0;
}
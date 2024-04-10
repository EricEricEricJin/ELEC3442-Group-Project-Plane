#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    void* p = malloc(1024);
    int s = sizeof(p);
    printf("%d", s);
    return 0;
}
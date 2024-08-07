#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid = getpid();
    printf("%d\n", pid);
    return 0;
}

// output
// 24475

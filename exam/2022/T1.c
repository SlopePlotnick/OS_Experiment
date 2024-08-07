#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

void delay()
{
    int i;
    for (i = 0; i < 87654321; i++)
        ;
}

void master(int *fd)
{
    close(fd[0]);
    for (int i = 0; i < 8; i++)
    {
        char item = 'a' + i;
        char str[2];
        str[0] = item;
        str[1] = '\0';
        write(fd[1], str, 1);
        delay();
    }
    write(fd[1], "z", 1);
    delay();
    write(fd[1], "z", 1);
    delay();
    close(fd[1]);
}

void child(int *fd)
{
    close(fd[1]);
    pid_t pid = getpid();

    char buf[2];
    read(fd[0], buf, 1);
    delay();
    while (1)
    {
        if (buf[0] == 'z')
        {
            close(fd[0]);
            exit(0);
        }

        buf[0] = toupper(buf[0]);
        printf("%d: %c\n", pid, buf[0]);
        read(fd[0], buf, 1);
        delay();
    }
}

int main()
{
    int fd[2];

    pipe(fd);
    for (int i = 0; i < 2; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            child(fd);
        }
    }
    master(fd);
    wait(NULL);
    wait(NULL);
    printf("Produce END\n");

    return 0;
}
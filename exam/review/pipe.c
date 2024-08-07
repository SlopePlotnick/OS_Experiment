#include <stdio.h>
#include <unistd.h>

void bad()
{
    int fd[2];
    pid_t pid;

    pipe(fd);
    pid = fork();
    if (pid == 0) {
        write(fd[1], "abc\n", 4);
    } else {
        char buff[1];
        int count;
        while (1) {
            count = read(fd[0], buff, sizeof(buff));
            if (count == 0)
                break;
            write(1, buff, count);
        }
    }
}

void good()
{
    int fd[2];
    pid_t pid;

    pipe(fd);
    pid = fork();
    if (pid == 0) {
        close(fd[0]);
        write(fd[1], "abc\n", 4);
    } else {
        close(fd[1]);
        char buff[1];
        int count;
        while (1) {
            count = read(fd[0], buff, sizeof(buff));
            if (count == 0)
                break;
            write(1, buff, count);
        }
    }
}

int main()
{
    good();
    return 0;
}

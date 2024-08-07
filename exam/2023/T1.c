#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
    int pid;
    int fdp[2];
    int fdq[2];
    pipe(fdp);
    pipe(fdq);
    pid = fork();

    if (pid == 0)
    {
        close(fdp[1]);
        dup2(fdp[0], 0);
        close(fdp[0]);

        close(fdq[0]);
        dup2(fdq[1], 1);
        close(fdq[1]);

        char buff[10];
        read(0, buff, 4);
        for (int i = 0; i < 4; i++)
        {
            buff[i] = toupper(buff[i]);
        }
        write(1, buff, 4);
        exit(0);
    }
    close(fdp[0]);
    dup2(fdp[1], 1);
    close(fdp[1]);

    close(fdq[1]);
    dup2(fdq[0], 0);
    close(fdq[0]);

    char buff[10];
    write(1, "abc\n", 4);
    wait(NULL);
    read(0, buff, 4);
    write(2, buff, 4);

    return 0;
}
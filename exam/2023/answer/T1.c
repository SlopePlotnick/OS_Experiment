#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

void convert(char *buff, int count)
{
    int i;
    for (i = 0; i < count; i++)
        if (islower(buff[i]))
            buff[i] = toupper(buff[i]);
}

void child(int *p, int *q)
{
    char buff[100];
    int count;

    dup2(p[0], 0);
    dup2(q[1], 1);
    close(p[0]);
    close(p[1]);
    close(q[0]);
    close(q[1]);

    count = read(0, buff, sizeof(buff));
    convert(buff, count);
    write(1, buff, count); 
}

void parent(int *p, int *q)
{
    char buff[100];
    int count;

    dup2(p[1], 1);
    dup2(q[0], 0);
    close(p[0]);
    close(p[1]);
    close(q[0]);
    close(q[1]);

    write(1, "abc\n", 4);
    count = read(0, buff, sizeof(buff));
    write(2, buff, count);
}

int main()
{
    int p[2];
    int q[2];

    pipe(p);
    pipe(q);

    pid_t pid = fork();
    if (pid == 0) {
        child(p, q);
        exit(0);
    }
    parent(p, q);
    return 0;
}

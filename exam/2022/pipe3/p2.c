#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    int fd1[2]; pipe(fd1);
    int fd2[2]; pipe(fd2);
    pid_t pid1, pid2;

    pid1 = fork();
    if(pid1 == 0){
        dup2(fd1[1], 1); close(fd1[0]); close(fd1[1]);
        execlp("cat", "cat", "/etc/passwd", NULL);
        exit(0);
    }

    if(pid2 == 0){
        dup2(fd1[0], 0); close(fd1[0]); close(fd1[1]);
        dup2(fd2[1], 1); close(fd2[0]); close(fd2[1]);
        execlp("grep", "grep", "root", NULL);
        exit(0);
    }

    dup2(fd2[0], 0); close(fd2[0]); close(fd2[1]);
    execlp("wc", "wc", "-l", NULL);
    return 0;
}

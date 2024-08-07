// p1.c 存在的问题
// + 主进程一开始就调用了 2 次 pipe
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    // + 主进程一开始就调用了 2 次 pipe
    int fd1[2]; pipe(fd1);
    int fd2[2]; pipe(fd2);
    pid_t pid1, pid2;

    pid1 = fork();
    if(pid1 == 0){
        // + 第一个子进程继承了 fd1[0] fd1[1] fd2[0] fd2[1] 4 个文件描述符
        //   - 需要关闭 4 个文件描述符
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

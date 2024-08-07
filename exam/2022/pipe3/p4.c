// 示意图
//     x[1] x[0]      y[1] y[0] 
// cat           grep           wc

// + 通过良好的设计，提高程序的可读性，从而预先避免错误
//   - 简洁的变量名
//   - 示意图
//   - 编写小的函数
//
// + 连接管道，使用递归的代码最简洁

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

void start_cat(int x[2], int y[2])
{
    pid_t pid = fork();
    if (pid == 0) {
        dup2(x[1], 1); 
        close(x[0]);
        close(x[1]);
        close(y[0]);
        close(y[1]);

        execlp("cat", "cat", "/etc/passwd", NULL);
        exit(0);
    }
}

void start_grep(int x[2], int y[2])
{
    pid_t pid = fork();
    if (pid == 0) {
        dup2(x[0], 0);
        dup2(y[1], 1);
        close(x[0]);
        close(x[1]);
        close(y[0]);
        close(y[1]);

        execlp("grep", "grep", "root", NULL);
        exit(0);
    }
}

void start_wc(int x[2], int y[2])
{
    dup2(y[0], 0);
    close(x[0]);
    close(x[1]);
    close(y[0]);
    close(y[1]);

    execlp("wc", "wc", "-l", NULL);
}

//     x[1] x[0]      y[1] y[0] 
// cat           grep           wc
int main()
{
    int x[2];
    pipe(x);
    int y[2];
    pipe(y);

    start_cat(x, y);
    start_grep(x, y);
    start_wc(x, y);
    return 0;
}

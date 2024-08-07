#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

void mysys(char *command)
{
    // 实现该函数，该函数执行一条命令，并等待该命令执行结束
    // 1. 把 command 分割为多个单词
    //    + 保存到 words 数组中
    char cmd[100];
    strcpy(cmd, command);
    char *p = strtok(cmd, " ");
    char *words[100];
    int i = 0;
    while (p)
    {
        words[i++] = p;
        p = strtok(NULL, " ");
    }
    words[i] = NULL;
    // 2. fork 创建子进程
    pid_t pid;
    pid = fork();
    // 3. 子进程 execlp(words[0], words)
    if (!pid)
    {
        int error = execvp(words[0], words);
        if (error < 0)
            perror("execvp");
    }
    // 4. 父进程 wait 子进程结束
    else
        wait(NULL);
}

int main()
{
    // 不要修改 main 函数
    puts("---");
    mysys("echo HELLO WORLD");
    puts("---");
    mysys("ls /");
    puts("---");
    return 0;
}
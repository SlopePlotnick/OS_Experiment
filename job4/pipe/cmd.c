#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "cmd.h"

void exec_cmd(struct cmd *cmd)
{
    /* 完成此函数 */
    if (cmd->input)
    {
        int fd;
        fd = open(cmd->input, O_CREAT | O_RDONLY, 0777);
        dup2(fd, 0);
        close(fd);
    }
    if (cmd->output)
    {
        int fd;
        fd = open(cmd->output, O_CREAT | O_WRONLY, 0777);
        dup2(fd, 1);
        close(fd);
    }
    int error = execvp(cmd->argv[0], cmd->argv);
    if (error < 0)
        perror("exec");
    exit(0);
}

int builtin_cmd(struct cmd *cmd)
{
    /* 实现 cd 命令, 返回 1 */
    if (!strcmp(cmd->argv[0], "cd"))
    {
        int error = chdir(cmd->argv[1]);
        if (error < 0)
            perror("chdir");

        return 1;
    }

    /* 实现 pwd 命令, 返回 1 */
    if (!strcmp(cmd->argv[0], "pwd"))
    {
        char buf[100];
        memset(buf, 0, sizeof(buf));
        char *error = getcwd(buf, sizeof(buf));
        if (error == NULL)
            perror("getcwd");
        else
        {
            write(1, buf, sizeof(buf));
            write(1, "\n", 1);
        }
        return 1;
    }

    /* 实现 exit 命令, 返回 1 */
    if (!strcmp(cmd->argv[0], "exit"))
    {
        exit(0);

        return 1;
    }

    return 0;
}

void exec_pipe_cmd(int cmdc, struct cmd *cmdv)
{
    if (cmdc == 1)
        exec_cmd(&cmdv[0]);
    else if (cmdc == 2)
    {
        /* 处理管道中包含 2 个命令的情况 */
        int pid;
        int fd[2];

        pipe(fd);
        pid = fork();
        if (pid == 0)
        {
            // child
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);

            exec_cmd(&(cmdv[0]));
            exit(0);
        }
        // parent
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);

        exec_cmd(&(cmdv[1]));
    }
    else
    {
        // 多条指令的情况 采用递归思想
        int pid;
        int fd[2];

        pipe(fd);
        pid = fork();
        if (pid == 0)
        {
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);

            exec_pipe_cmd(cmdc - 1, cmdv);
            exit(0);
        }
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);

        exec_cmd(&(cmdv[cmdc - 1]));
    }
}
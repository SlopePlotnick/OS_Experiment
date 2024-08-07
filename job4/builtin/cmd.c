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
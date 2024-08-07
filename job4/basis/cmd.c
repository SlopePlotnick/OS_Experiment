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
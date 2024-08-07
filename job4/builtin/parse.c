#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"

void parse_cmd(char *line, struct cmd *cmd)
{
    cmd->argc = 0;

    /* 使用 strtok 完成此函数 */
    const char delim[] = " ";
    char *tok;
    tok = strtok(line, delim);
    while (tok != NULL)
    {
        cmd->argv[cmd->argc] = tok;
        cmd->argc++;
        tok = strtok(NULL, delim);
    }

    cmd->argv[cmd->argc] = NULL;
}

void dump_cmd(struct cmd *cmd)
{
    printf("argc = %d\n", cmd->argc);
    int i;
    for (i = 0; i < cmd->argc; i++)
    {
        printf("argv[%d] = (%s)\n", i, cmd->argv[i]);
    }
}
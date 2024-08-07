#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"

void parse_cmd(char *line, struct cmd *cmd)
{
    cmd->argc = 0;
    cmd->input = NULL;
    cmd->output = NULL;

    /* 使用 strtok 完成此函数 */
    const char delim[] = " ";
    char *tok;
    tok = strtok(line, delim);
    while (tok != NULL)
    {
        if (tok[0] != '<' && tok[0] != '>')
        {
            cmd->argv[cmd->argc] = tok;
            cmd->argc++;
        }
        else
        {
            if (tok[0] == '>')
            {
                cmd->output = tok + 1;
            }
            else
            {
                cmd->input = tok + 1;
            }
        }

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

    char *input = cmd->input ? cmd->input : "NULL";
    printf("input = (%s)\n", input);

    char *output = cmd->output ? cmd->output : "NULL";
    printf("output = (%s)\n", output);
}
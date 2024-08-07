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

int parse_pipe_cmd(char *line, struct cmd *cmdv)
{
    /* 使用 strtok_r 完成此函数 */
    char *token, *subtoken;
    char *saveptr;
    int cnt = 0;

    token = strtok_r(line, "|", &saveptr); // 第一次分割，使用'|'作为分隔符
    while (token != NULL)
    {
        // 此时token中保存着管道分割的一条完整命令
        // 将token传给parse_cmd进行二次分割
        parse_cmd(token, &(cmdv[cnt]));
        cnt++;

        token = strtok_r(NULL, "|", &saveptr);
    }

    return cnt; // 返回管道分割的命令个数
}

void dump_pipe_cmd(int cmdc, struct cmd *cmdv)
{
    int i;

    printf("pipe cmd, cmdc = %d\n", cmdc);
    for (i = 0; i < cmdc; i++)
    {
        struct cmd *cmd = cmdv + i;
        dump_cmd(cmd);
    }
}
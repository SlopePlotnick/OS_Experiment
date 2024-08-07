#ifndef _CMD_H
#define _CMD_H

#define MAX_ARGC 10

struct cmd
{
    int argc;
    char *argv[MAX_ARGC];
};

extern void exec_cmd(struct cmd *cmd);
extern int builtin_cmd(struct cmd *cmd);

#endif
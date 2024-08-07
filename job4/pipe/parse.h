#ifndef _PARSE_H
#define _PARSE_H

#include "cmd.h"

extern void parse_cmd(char *line, struct cmd *cmd);
extern void dump_cmd(struct cmd *cmd);
#define MAX_CMDC 20
extern int parse_pipe_cmd(char *line, struct cmd *cmdv);
extern void dump_pipe_cmd(int cmdc, struct cmd *cmdv);

#endif
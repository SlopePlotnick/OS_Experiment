#ifndef _CMD_H
#define _CMD_H

typedef struct {
    char *name;
    void (*handler)(int argc, char *argv[]);
} cmd_t;

extern void usage();
extern void do_cmd(int argc, char *argv[]);

#endif

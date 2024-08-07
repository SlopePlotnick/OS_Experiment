#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cmd.h"
#include "parse.h"

void read_line(char *line, int size)
{
    int count = read(0, line, size);
    assert(line[count - 1] == '\n');
    line[count - 1] = 0;
}

int main(int argc, char *argv[])
{
    while (1)
    {
        char line[100];
        write(1, "> ", 2);
        read_line(line, sizeof(line));

        struct cmd cmd;
        parse_cmd(line, &cmd);

        if (builtin_cmd(&cmd))
            continue;

        pid_t pid = fork();
        if (pid == 0)
        {
            exec_cmd(&cmd);
        }
        wait(NULL);
    }
    return 0;
}
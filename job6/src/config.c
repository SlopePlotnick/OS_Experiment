#include "utils/std.h"
#include "utils/file.h"
#include "oid.h"
#include "cmd.h"
#include "config.h"

void git_init(int argc, char *argv[])
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        int error = execlp("mkdir", "mkdir", ".dg", NULL);
        if (error < 0)
            perror("execl");
        exit(0);
    }
    else
    {
        wait(NULL);
        char *content = "0000000000" "0000000000" "0000000000" "0000000000\n";
        make_file("./.dg/HEAD", content);
        content = "tree\n0\n";
        make_file("./.dg/index", content);
        int error = execlp("mkdir", "mkdir", ".dg/objects", NULL);
        if (error < 0)
            perror("execl");
    }
}

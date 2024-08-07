#include "utils/std.h"
#include "cmd.h"
#include "config.h"
#include "index.h"
#include "commit.h"

void usage()
{
    puts("DeltaGit Usage");
    puts("  dg init");

    puts("  dg cat-file -t oid      打印类型");
    puts("  dg cat-file -s oid      打印大小");
    puts("  dg cat-file -p oid      打印内容");

    puts("  dg add file             把文件加入到索引区");
    puts("  dg add dir              把目录下的文件加入到索引区");
    puts("  dg rm file              从索引区中删除文件");
    puts("  dg rm dir               从索引区中删除目录下的文件");
    puts("");

    puts("  dg commit -m msg");
    puts("  dg checkout oid");
    puts("  dg log");

    exit(EXIT_FAILURE);
}

cmd_t cmd_vector[] = {
    {"init", git_init},    
    {"add", git_add},
    {"rm", git_rm},
    {"commit", git_commit},
    {"log", git_log},
    {"checkout", git_checkout},
    {NULL, NULL}
};

void do_cmd(int argc, char *argv[])
{
    char *cmd_name = argv[0];

    cmd_t *cmd;
    for (cmd = cmd_vector; cmd->name; cmd++) {
        if (strcmp(cmd->name, cmd_name) == 0) {
            cmd->handler(argc, argv);
            return;
        }
    }

    usage();
}

#include "utils/std.h"
#include "cmd.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
        usage();
    do_cmd(argc - 1, argv + 1);
    return 0;
}

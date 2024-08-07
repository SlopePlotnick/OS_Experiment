#include "utils/std.h"
#include "utils/file.h"
#include "config.h"
#include "head.h"

void head_load(char *line)
{
    load_line(GIT_HEAD_PATH, line);
}

void head_store(char *line)
{
    store_line(GIT_HEAD_PATH, line);
}

void head_load_oid(oid_t *oid)
{
    char line[LINE_SIZE];
    load_line(GIT_HEAD_PATH, line);
    oid_set(oid, line);
}

void head_store_oid(oid_t *oid)
{
    char line[LINE_SIZE];
    load_line(GIT_HEAD_PATH, line);
    store_line(GIT_HEAD_PATH, oid->data);
}

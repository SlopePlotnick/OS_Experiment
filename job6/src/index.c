#include "utils/std.h"
#include "utils/file.h"
#include "utils/dir.h"
#include "cmd.h"
#include "config.h"
#include "tree.h"
#include "oid.h"
#include "index.h"

tree_t *index_load()
{
    tree_t *this = tree_read(GIT_INDEX_PATH);
    return this;
}

void index_store(tree_t *this)
{
    tree_write(this, GIT_INDEX_PATH);
}

void index_dup(oid_t *oid)
{
    get_file_oid(GIT_INDEX_PATH, oid);

    char target_path[PATH_MAX];
    get_oid_path(oid, target_path);

    copy_file(GIT_INDEX_PATH, target_path);
}

void git_add(int argc, char *argv[])
{
    tree_t *idx = index_load();

    for (int i = 1; i <= argc - 1; i++)
    {
        tree_add(idx, argv[i]);
    }

    index_store(idx);
}

void git_rm(int argc, char *argv[])
{
    tree_t *idx = index_load();

    for (int i = 1; i <= argc - 1; i++)
    {
        tree_remove(idx, argv[i]);
    }

    index_store(idx);
}

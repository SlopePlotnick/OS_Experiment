#include "utils/std.h"
#include "utils/file.h"
#include "utils/dir.h"
#include "config.h"
#include "cmd.h"
#include "head.h"
#include "index.h"
#include "blob.h"
#include "commit.h"

commit_t *commit_new(void)
{
    commit_t *this = malloc(sizeof(commit_t));
    return this;
}

void commit_delete(commit_t *this)
{
    free(this);
}

void commit_dump(commit_t *this)
{
    printf("commit %s\n", this->oid.data);
    printf("time   %s\n", this->time);
    printf("msg    %s\n", this->msg);
    printf("\n");
}

commit_t *commit_load(oid_t *oid)
{
    commit_t *this = malloc(sizeof(commit_t));
    this->oid = *oid;

    FILE *f = fopen_oid(oid, "r");
    char line[LINE_SIZE];

    // 1. commit
    read_line(f, line);

    // 2. size
    read_line(f, line);

    // 3. parent_oid
    read_line(f, line);
    oid_set(&this->parent_oid, line);

    // 4. tree_oid
    read_line(f, line);
    oid_set(&this->tree_oid, line);

    // 5. time
    read_line(f, line);
    strcpy(this->time, line);

    // 6. message
    read_line(f, line);
    strcpy(this->msg, line);

    fclose(f);
    return this;
}

void commit_make_text(commit_t *this, char *text)
{
    char *p = text;

    p += sprintf(p, "commit\n");
    p += sprintf(p, "0\n");
    p += sprintf(p, "%s\n", this->parent_oid.data);
    p += sprintf(p, "%s\n", this->tree_oid.data);
    p += sprintf(p, "%s\n", this->time);
    p += sprintf(p, "%s\n", this->msg);
}

void commit_make_time(commit_t *this)
{
    time_t time_long = time(NULL);
    char *time_text = ctime(&time_long);
    int time_last = strlen(time_text) - 1;
    time_text[time_last] = 0;
    strcpy(this->time, time_text);
}

void commit_store(commit_t *this, oid_t *oid)
{
    char text[4096];
    commit_make_text(this, text);

    get_string_oid(text, oid);
    this->oid = *oid;
    printf("commit %s %s\n", oid->data, this->msg);

    FILE *f = fopen_oid(oid, "w");
    fputs(text, f);
    fclose(f);
}

void git_commit(int argc, char *argv[])
{
    oid_t *old_com_oid = malloc(sizeof(oid_t));
    head_load_oid(old_com_oid);
    oid_t *tree_oid = malloc(sizeof(oid_t));
    index_dup(tree_oid);

    commit_t *com = commit_new();
    strcpy(com->parent_oid.data, old_com_oid->data);
    strcpy(com->tree_oid.data, tree_oid->data);
    char *msg = argv[2];
    strcpy(com->msg, msg);
    commit_make_time(com);

    oid_t *new_com_oid = malloc(sizeof(oid_t));
    commit_store(com, new_com_oid);

    head_store_oid(new_com_oid);
}

void git_log(int argc, char *argv[])
{
    oid_t *cur_oid = malloc(sizeof(oid_t));
    head_load_oid(cur_oid);

    int start = 1;
    while (!oid_is_zero(cur_oid))
    {
        commit_t *com = commit_new();
        com = commit_load(cur_oid);

        if (!start)
            printf("\n");
        printf("commit %s\n", com->oid.data);
        printf("time   %s\n", com->time);
        printf("msg    %s\n", com->msg);

        start = 0;

        cur_oid = &(com->parent_oid);
    }
}

void empty_dir(char *path)
{
    dir_t *dir = dir_open(path);
    entry_t entry;

    while (dir_read(dir, &entry))
    {
        if (strcmp(entry.name, GIT_DIR) == 0)
            continue;

        char *sub_path = entry.path;
        printf("unlnk %s\n", sub_path);

        if (entry_is_dir(&entry))
        {
            empty_dir(sub_path);
            rmdir(sub_path);
        }
        else
        {
            unlink(sub_path);
        }
    }

    dir_close(dir);
}

void git_checkout(int argc, char *argv[])
{
    if (argc != 2)
        usage();

    empty_dir(".");

    char *abbr = argv[1];
    oid_t oid; // 要checkout的版本oid
    make_oid(abbr, &oid);

    commit_t *com = commit_new();
    com = commit_load(&oid);
    tree_t *tr = tree_new();
    oid_t tr_oid = com->tree_oid;
    tr = tree_load(&tr_oid);

    node_t *node;
    leaf_t *leaf;
    list_each(&tr->leaf_list, node, leaf)
    {
        if (oid_is_zero(&(leaf->oid)))
        {
            // 文件夹 进行创建
            mkdir(leaf->path, 0777);
        }
        else
        {
            copy_blob_content(&(leaf->oid), leaf->path);
        }
    }

    head_store_oid(&oid);
}

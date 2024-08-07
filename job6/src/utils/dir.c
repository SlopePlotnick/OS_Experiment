#include "std.h"
#include "dir.h"

int entry_is_regular(entry_t *this)
{
    return this->type == DT_REG;
}

int entry_is_dir(entry_t *this)
{
    return this->type == DT_DIR;
}

dir_t *dir_open(char *path)
{
    dir_t *this = malloc(sizeof(dir_t));
    strcpy(this->base, path);
    this->fd = opendir(path);
    return this;
}

// 1. 忽略 . 和 ..
// 2. 处理 ./dir/file 为 dir/file 
int dir_read(dir_t *this, entry_t *entry)
{
    struct dirent *de;

    while (1) {
        de = readdir(this->fd);
        if (de == NULL)
            return 0;
        int type = de->d_type;
        char *name = de->d_name;

        if (strcmp(name, ".") == 0)
            continue;

        if (strcmp(name, "..") == 0)
            continue;

        entry->type = type;
        strcpy(entry->name, name);

        if (strcmp(this->base, ".") == 0)
            strcpy(entry->path, name);
        else
            sprintf(entry->path, "%s/%s", this->base, name);
        return 1;
    }
}

void dir_close(dir_t *this)
{
    closedir(this->fd);
}

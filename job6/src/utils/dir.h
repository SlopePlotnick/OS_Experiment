#ifndef _UTILS_DIR_H
#define _UTILS_DIR_H

typedef struct {
    int type;
    char name[NAME_MAX];
    char path[PATH_MAX];
} entry_t;

extern int entry_is_regular(entry_t *this);
extern int entry_is_dir(entry_t *this);

typedef struct {
    char base[PATH_MAX];
    DIR *fd;
} dir_t;

extern dir_t *dir_open(char *path);
extern int dir_read(dir_t *this, entry_t *entry);
extern void dir_close(dir_t *this);

#endif

#ifndef _FS_DIR_H
#define _FS_DIR_H

#include "inode.h"
#include <limits.h>

typedef struct dir_entry {
    char name[28];
    int ino;
} dir_entry_t;

extern void dir_entry_set(dir_entry_t *dir_entry, char *name, int ino);
extern void dir_add_dot(inode_t *this, int this_ino, int parent_ino);
extern int dir_add_entry(inode_t *this, char *name, int ino);
extern int dir_remove_entry(inode_t *this, char *name);

struct buff;
typedef struct {
    inode_t *inode;
    int offset;
    void *buff;
} dir_cursor_t;

extern void dir_cursor_init(dir_cursor_t *this, inode_t *inode);
extern dir_entry_t *dir_cursor_next(dir_cursor_t *this);
extern void dir_cursor_destroy(dir_cursor_t *this);

extern int dir_make_regular(inode_t *this, char *name, inode_t **result);
extern int dir_make_dir(inode_t *this, char *name, inode_t **result);
extern inode_t *dir_lookup_inode(inode_t *this, char *name);
extern bool dir_lookup_ino(inode_t *this, int ino, dir_entry_t *result);

enum {
    NAMEI_OPEN,
    NAMEI_MAKE,
};

extern int namei(int flag, const char *path, inode_t **ip, char *base_name);
extern int namei_open(const char *path, inode_t **ip);
extern int namei_make(const char *path, inode_t **ip, char *base_name);

#endif

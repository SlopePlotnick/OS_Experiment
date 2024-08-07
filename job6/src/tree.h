#ifndef _TREE_H
#define _TREE_H

#include "utils/list.h"
#include "oid.h"

typedef struct {
    char *path;
    oid_t oid;
} leaf_t;

typedef struct {
    oid_t oid;
    list_t leaf_list;
} tree_t;

extern tree_t *tree_new();
extern void tree_delete(tree_t *this);
extern tree_t *tree_read(char *path);
extern void tree_write(tree_t *this, char *path);
extern tree_t *tree_load(oid_t *oid);
extern void tree_store(tree_t *this, oid_t *oid);
extern void tree_add(tree_t *this, char *path);
extern void tree_remove(tree_t *this, char *path);
extern oid_t *tree_find(tree_t *this, char *path);

#endif

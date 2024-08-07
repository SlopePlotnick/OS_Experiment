#ifndef _INDEX_H
#define _INDEX_H

#include "oid.h"
#include "tree.h"

extern tree_t *index_load();
extern void index_store(tree_t *this);
extern void index_dup(oid_t *oid);
extern void git_add(int argc, char *argv[]);
extern void git_rm(int argc, char *argv[]);

#endif

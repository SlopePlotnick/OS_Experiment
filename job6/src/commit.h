#ifndef _COMMIT_H
#define _COMMIT_H

#include "oid.h"

#define MAX_MSG_SIZE 512

typedef struct {
    oid_t oid;
    oid_t parent_oid;
    oid_t tree_oid;
    char time[64];
    char msg[MAX_MSG_SIZE];
} commit_t;

extern commit_t *commit_new(void);
extern void commit_delete(commit_t *this);
extern void commit_dump(commit_t *this);
extern commit_t *commit_load(oid_t *oid);
extern void commit_store(commit_t *this, oid_t *oid);
extern void git_commit(int argc, char *argv[]);
extern void git_log(int argc, char *argv[]);
extern void git_checkout(int argc, char *argv[]);

#endif

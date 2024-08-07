#ifndef _BLOB_H
#define _BLOB_H

#include "oid.h"

typedef struct {
    char *oid;
} blob_t;

extern bool store_blob(char *source_path, oid_t *oid);
extern void copy_blob_content(oid_t *oid, char *target_path);

#endif

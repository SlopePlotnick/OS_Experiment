#ifndef _HEAD_H
#define _HEAD_H

#include "oid.h"
#include "tree.h"

extern void head_load(char *line);
extern void head_store(char *line);
extern void head_load_oid(oid_t *oid);
extern void head_store_oid(oid_t *oid);

#endif

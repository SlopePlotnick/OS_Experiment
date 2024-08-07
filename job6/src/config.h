#ifndef _CONFIG_H
#define _CONFIG_H

#define GIT_DIR             ".dg"

#define GIT_HEAD_PATH       ".dg/HEAD"
#define GIT_OBJECTS_DIR     ".dg/objects"
#define GIT_INDEX_PATH      ".dg/index"

extern void git_init(int argc, char *argv[]);

#endif

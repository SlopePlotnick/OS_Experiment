#ifndef _DISK_H
#define _DISK_H

typedef struct {
    int fd;
    char *data;
    int size;
} disk_t;

extern void disk_create(char *path, int size);
extern void *disk_open(char *path);
extern void disk_close();
extern void *disk_seek_byte(int pos);
extern void *disk_seek_block(int bno);

#endif

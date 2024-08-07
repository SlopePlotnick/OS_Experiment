#ifndef _FS_INODE_H
#define _FS_INODE_H

struct fs;

#define DIRECT_BLOCK_COUNT 10 
typedef struct inode {
    int link_count;
    int mode;
    int uid;
    int gid;
    int real_device;
    int size;
    int address[DIRECT_BLOCK_COUNT];
} inode_t;

extern void inode_init(inode_t *this, int mode);
extern int inode_is_dir(inode_t *this);
extern int inode_is_regular(inode_t *this);
extern int inode_map(inode_t *this, int block);
extern int inode_read(inode_t *this, off_t position, void *memory, int size);
extern int inode_write(inode_t *this, off_t position, void *memory, int size);
extern int inode_trunc(inode_t *this);
extern int inode_stat(inode_t *this, struct stat *buf);
extern void inode_dump(inode_t *this);

#endif

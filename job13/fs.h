#ifndef _KERNEL_FS_H
#define _KERNEL_FS_H

struct inode;
#define BLOCK_SIZE 512

typedef struct {
    int start_bno;
    int block_count;
} region_t;

extern void region_init(region_t *this, int start_bno, int block_count);
extern void region_dump(region_t *this, char *title);

#define FS_MAGIC 0xAABBCCDD
typedef struct fs {
    int magic;
    int inode_count;
    int data_count;
    region_t inode_bitmap;
    region_t inode_table;
    region_t data_bitmap;
    region_t data_table;
} fs_t;

extern fs_t *fs;
// fs inode_bitmap inode_table data_bitmap data_table

#define FAKE_INO -1
#define FAKE_BNO -1
#define ROOT_INO 0

#define BITS_PER_BLOCK (BLOCK_SIZE * 8)
#define INODES_PER_BLOCK (BLOCK_SIZE / sizeof(inode_t))

extern int fs_alloc_ino();
extern void fs_free_ino(int ino);
extern int fs_alloc_bno();
extern void fs_free_bno(int bno);

extern struct inode *fs_seek_inode(int ino);
extern int fs_seek_ino(struct inode *inode);

extern void fs_mount();
extern void fs_dump();
extern void fs_make(char *path, int inode_count, int data_count);

extern struct inode *root_dir;
extern struct inode *work_dir;

#endif

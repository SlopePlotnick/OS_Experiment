#include "utils/std.h"
#include "inode.h"
#include "dir.h"
#include "disk.h"
#include "fs.h"

void fs_make_root_dir()
{
    int ino = fs_alloc_ino();
    allege(ino == ROOT_INO);

    root_dir = fs_seek_inode(ROOT_INO);
    inode_init(root_dir, S_IFDIR | 0666);
    dir_add_dot(root_dir, ROOT_INO, ROOT_INO);
}

void fs_make_disk(char *path, int inode_count, int data_count)
{
    int disk_size = 0; 
    disk_size += BLOCK_SIZE;                    // super block
    disk_size += inode_count / 8;               // inode bitmap
    disk_size += inode_count * sizeof(inode_t); // inode table
    disk_size += data_count / 8;                // data bitmap
    disk_size += data_count * BLOCK_SIZE;       // data table
    disk_create(path, disk_size);
}

void fs_make_regions(char *path, int inode_count, int data_count)
{
    int start_bno = 1;
    region_t *region;

    // inode_bitmap
    region = &fs->inode_bitmap;
    region_init(region, start_bno, inode_count / BITS_PER_BLOCK);
    start_bno += region->block_count;

    // inode_table
    region = &fs->inode_table;
    region_init(region, start_bno, inode_count / INODES_PER_BLOCK);
    start_bno += region->block_count;

    // data_bitmap
    region = &fs->data_bitmap;
    region_init(region, start_bno, data_count / BITS_PER_BLOCK);
    start_bno += region->block_count;

    // data_table
    region = &fs->data_table;
    region_init(region, start_bno, data_count);
    start_bno += region->block_count;
}

void fs_make(char *path, int inode_count, int data_count)
{
    assert(BLOCK_SIZE % sizeof(inode_t) == 0);
    assert(BLOCK_SIZE % sizeof(dir_entry_t) == 0);
    assert(inode_count % BITS_PER_BLOCK == 0);
    assert(data_count % BITS_PER_BLOCK == 0);

    fs_make_disk(path, inode_count, data_count);

    fs = disk_open(path);
    fs->magic = FS_MAGIC;
    fs->inode_count = inode_count;
    fs->data_count = data_count;

    fs_make_regions(path, inode_count, data_count);
    fs_make_root_dir();
    disk_close();
}

void mkfs_uxfs(int argc, char *argv[])
{
    if (argc != 2) {
        puts("Usage: mkfs.uxfs path");
        return;
    }

    int inode_count = 2 * BLOCK_SIZE * 8;
    int data_count = 2 * BLOCK_SIZE * 8;

    char *path = argv[1];
    fs_make(path, inode_count, data_count);
}

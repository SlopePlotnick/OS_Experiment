#include "utils/std.h"
#include "utils/bitmap.h"
#include "disk.h"
#include "inode.h"
#include "dir.h"
#include "fs.h"

fs_t *fs;
inode_t *root_dir;
inode_t *work_dir;

void region_init(region_t *this, int start_bno, int block_count)
{
    this->start_bno = start_bno;
    this->block_count = block_count;
}

void region_dump(region_t *this, char *title)
{
    printf("%-13s: start_bno = %4d, block_count = %4d\n", 
            title, this->start_bno, this->block_count);
}

int region_alloc_bit(region_t *region)
{
    void *bitmap = disk_seek_block(region->start_bno);
    int bitmap_size = region->block_count * BLOCK_SIZE;

    int bit = bitmap_alloc_bit(bitmap, bitmap_size);
    return bit;
}

void region_free_bit(region_t *region, int bit)
{
    void *bitmap = disk_seek_block(region->start_bno);
    int bitmap_size = region->block_count * BLOCK_SIZE;

    assert(bit < bitmap_size * 8);
    bitmap_free_bit(bitmap, bit);
}

int fs_alloc_ino()
{
    region_t *region = &fs->inode_bitmap;
    int ino = region_alloc_bit(region);
    if (ino < 0)
        panic("alloc ino failed");
    return ino;
}

void fs_free_ino(int ino)
{
    assert(ino < fs->inode_count);
    region_t *region = &fs->inode_bitmap;
    region_free_bit(region, ino);
}

int fs_alloc_bno()
{
    region_t *region = &fs->data_bitmap;
    int bno = region_alloc_bit(region);
    if (bno < 0)
        panic("alloc bno failed");
    return fs->data_table.start_bno + bno;
}

void fs_free_bno(int bno)
{
    assert(bno != FAKE_BNO);
    int bit = bno - fs->data_table.start_bno;
    assert(bit < fs->data_count);
    region_t *region = &fs->data_bitmap;
    region_free_bit(region, bit);
}

struct inode *fs_seek_inode(int ino)
{
    assert(ino < fs->inode_count);
    inode_t *inode_table = disk_seek_block(fs->inode_table.start_bno);
    inode_t *inode = inode_table + ino;
    return inode;
}

int fs_seek_ino(inode_t *inode)
{
    inode_t *inode_table = disk_seek_block(fs->inode_table.start_bno);
    return inode - inode_table;
}

void fs_mount()
{
    fs = disk_seek_byte(0);
    root_dir = fs_seek_inode(ROOT_INO);
    work_dir = root_dir;
}

void fs_dump()
{
    assert(fs->magic == FS_MAGIC);
    printf("BLOCK_SIZE = %d\n", BLOCK_SIZE);
    printf("magic = %x\n", fs->magic);
    printf("inode_count = %d\n", fs->inode_count);
    printf("data_count = %d\n", fs->data_count);

    region_dump(&fs->inode_bitmap, "inode bitmap");
    region_dump(&fs->inode_table, "inode table");
    region_dump(&fs->data_bitmap, "data bitmap");
    region_dump(&fs->data_table, "data table");
}

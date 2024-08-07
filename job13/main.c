#define FUSE_USE_VERSION 26
#include <fuse.h>
#include "utils/std.h"
#include "fs.h"
#include "mkfs.h"
#include "dump.h"
#include "inode.h"
#include "dir.h"
#include "disk.h"
#include "log.h"

int uxfs_rename(const char *old_path, const char *new_path)
{
    log_printf("rename %s %s\n", old_path, new_path);
    return 0;
}

int uxfs_chmod(const char *path, mode_t mode)
{
    log_printf("chmod %s\n", path);
    return 0;
}

int uxfs_access(const char *path, int mask)
{
    log_printf("access %s\n", path);
    return 0;
}

int uxfs_create(const char *path, mode_t mode, struct fuse_file_info *info)
{
    log_printf("create %s\n", path);

    int error;
    inode_t *dir;
    char base_name[NAME_MAX];

    error = namei_make(path, &dir, base_name);
    if (error < 0)
        return error;

    inode_t *leaf;
    error = dir_make_regular(dir, base_name, &leaf);
    if (error < 0)
        return error;

    info->fh = (intptr_t)leaf;
    return 0;
}

int uxfs_open(const char *path, struct fuse_file_info *info)
{
    log_printf("open %s\n", path);

    inode_t *leaf;
    int error = namei_open(path, &leaf);
    if (error < 0)
        return error;

    info->fh = (intptr_t)leaf;
    return 0;
}

int uxfs_release(const char *path, struct fuse_file_info *info)
{
    log_printf("release %s\n", path);
    info->fh = 0;
    return 0;
}

int uxfs_read(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *info)
{
    log_printf("read %s %ld at %d\n", path, size, offset);
    inode_t *file_inode = (inode_t *)(intptr_t)info->fh;
    int count = 0;
    count = inode_read(file_inode, offset, buf, size);
    return count;
}

int uxfs_write(const char *path, const char *buf, size_t size, off_t offset,
               struct fuse_file_info *info)
{
    log_printf("write %s at %d\n", path, offset);
    inode_t *file_inode = (inode_t *)(intptr_t)info->fh;
    int count = 0;
    count = inode_write(file_inode, offset, (void*)buf, size);
    return count;
}

int uxfs_mknod(const char *path, mode_t mode, dev_t dev)
{
    log_printf("mknod %s\n", path);
    return 0;
}

int uxfs_unlink(const char *path)
{
    log_printf("unlink %s\n", path);

    int error;
    inode_t *dir;
    char base_name[NAME_MAX];

    error = namei_make(path, &dir, base_name);
    if (error < 0)
        return error;

    error = dir_remove_entry(dir, base_name);
    if (error < 0)
        return error;

    int ino = error;
    inode_t *leaf = fs_seek_inode(ino);
    if (inode_is_dir(leaf))
        return -EISDIR;

    inode_trunc(leaf);
    fs_free_ino(ino);
    return 0;
}

int uxfs_mkdir(const char *path, mode_t mode)
{
    log_printf("mkdir %s\n", path);

    int error;
    inode_t *dir;
    char base_name[NAME_MAX];

    error = namei_make(path, &dir, base_name);
    if (error < 0)
        return error;

    inode_t *leaf;
    error = dir_make_dir(dir, base_name, &leaf);
    if (error < 0)
        return error;

    return 0;
}

int uxfs_rmdir(const char *path)
{
    log_printf("rmdir %s\n", path);

    int error;
    inode_t *dir;
    char base_name[NAME_MAX];

    error = namei_make(path, &dir, base_name);
    if (error < 0)
        return error;

    error = dir_remove_entry(dir, base_name);
    if (error < 0)
        return error;

    int ino = error;
    inode_t *leaf = fs_seek_inode(ino);
    if (inode_is_regular(leaf))
        return -ENOTDIR;

    inode_trunc(leaf);
    fs_free_ino(ino);
    return 0;
}

int uxfs_opendir(const char *path, struct fuse_file_info *info)
{
    log_printf("opendir %s\n", path);

    inode_t *leaf;
    int error = namei_open(path, &leaf);
    if (error < 0)
        return error;

    if (!S_ISDIR(leaf->mode))
        return -ENOTDIR;

    info->fh = (intptr_t)leaf;
    return 0;
}

int uxfs_releasedir(const char *path, struct fuse_file_info *info)
{
    log_printf("releasedir %s\n", path);
    info->fh = 0;
    return 0;
}

int uxfs_readdir(const char *path, void *buf, fuse_fill_dir_t fill,
                 off_t offset, struct fuse_file_info *info)
{
    inode_t *dir = (inode_t *)(intptr_t)(info->fh);
    log_printf("readdir %s\n", path);

    dir_entry_t *dir_entry;
    dir_cursor_t dir_cursor;
    dir_cursor_init(&dir_cursor, dir);

    while (dir_entry = dir_cursor_next(&dir_cursor))
    {
        if (dir_entry->ino != FAKE_INO)
        {

            fill(buf, dir_entry->name, NULL, 0);
        }
    }

    dir_cursor_destroy(&dir_cursor);
    return 0;
}

int uxfs_getattr(const char *path, struct stat *buf)
{
    log_printf("getattr %s\n", path);

    inode_t *leaf;
    int error = namei_open(path, &leaf);
    if (error < 0)
        return error;
    inode_stat(leaf, buf);
    return 0;
}

void uxfs_destroy(void *userdata)
{
    log_printf("destroy");
    disk_close();
}

struct fuse_operations uxfs_operations = {
    .rename = uxfs_rename,
    .chmod = uxfs_chmod,
    .access = uxfs_access,

    .mkdir = uxfs_mkdir,
    .rmdir = uxfs_rmdir,
    .opendir = uxfs_opendir,
    .releasedir = uxfs_releasedir,
    .readdir = uxfs_readdir,

    .open = uxfs_open,
    .create = uxfs_create,
    .mknod = uxfs_mknod,
    .unlink = uxfs_unlink,
    .release = uxfs_release,
    .read = uxfs_read,
    .write = uxfs_write,

    .getattr = uxfs_getattr,
    .destroy = uxfs_destroy};

int fuse_start(char *program, char *mount_point)
{
    char *argv[] = {
        program, "-s", "-f", "-o", "auto_unmount",
        mount_point, NULL};

    int argc = 0;
    while (argv[argc])
        argc++;
    return fuse_main(argc, argv, &uxfs_operations, NULL);
}

int main(int argc, char *argv[])
{
    if (strstr(argv[0], "mkfs.uxfs"))
    {
        mkfs_uxfs(argc, argv);
        exit(0);
    }

    if (strstr(argv[0], "dump.uxfs"))
    {
        dump_uxfs(argc, argv);
        exit(0);
    }

    if (argc < 3)
    {
        puts("Usage: uxfs -log disk_path mount_point");
        exit(0);
    }

    log_on = 0;
    for (int i = 1; i < argc; i++)
    {
        char *arg = argv[i];
        if (strcmp(arg, "-log") == 0)
            log_on = 1;
    }

    char *disk_path = argv[argc - 2];
    char *mount_point = argv[argc - 1];

    disk_open(disk_path);
    fs_mount();

    return fuse_start(argv[0], mount_point);
}

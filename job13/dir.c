#include "utils/std.h"
#include "utils/split.h"
#include "fs.h"
#include "dir.h"
#include "disk.h"

void dir_entry_set(dir_entry_t *this, char *name, int ino)
{
    strcpy(this->name, name);
    this->ino = ino;
}

void dir_entry_reset(dir_entry_t *this)
{
    this->name[0] = 0;
    this->ino = FAKE_INO;
}

void dir_cursor_init(dir_cursor_t *this, inode_t *inode)
{
    this->inode = inode;
    this->offset = 0;
    this->buff = NULL;
}

dir_entry_t *dir_cursor_next(dir_cursor_t *this)
{
    if (this->offset >= this->inode->size)
        return NULL;

    if (this->offset % BLOCK_SIZE == 0) {
        int bno = inode_map(this->inode, this->offset / BLOCK_SIZE);
        this->buff = disk_seek_block(bno);
    }

    dir_entry_t *dir_entry = this->buff + (this->offset % BLOCK_SIZE);
    this->offset += sizeof(dir_entry_t);
    return dir_entry;
}

void dir_cursor_destroy(dir_cursor_t *this)
{
}

bool dir_lookup_name(inode_t *this, char *name, dir_entry_t *result)
{
    bool found = 0;
    dir_entry_t *dir_entry;
    dir_cursor_t dir_cursor;

    dir_cursor_init(&dir_cursor, this);
    while (dir_entry = dir_cursor_next(&dir_cursor)) {
        if (dir_entry->ino == FAKE_INO)
            continue;
        if (strcmp(dir_entry->name, name) == 0) {
            found = 1;
            *result = *dir_entry;
            break;
        }
    }
    dir_cursor_destroy(&dir_cursor);

    return found;
}

bool dir_lookup_ino(inode_t *this, int ino, dir_entry_t *result)
{
    bool found = 0;
    dir_entry_t *dir_entry;
    dir_cursor_t dir_cursor;

    dir_cursor_init(&dir_cursor, this);
    while (dir_entry = dir_cursor_next(&dir_cursor)) {
        if (dir_entry->ino == FAKE_INO)
            continue;
        if (dir_entry->ino == ino) {
            found = 1;
            *result = *dir_entry; 
            break;
        }
    }
    dir_cursor_destroy(&dir_cursor);
    return found;
}

inode_t *dir_lookup_inode(inode_t *this, char *name)
{
    dir_entry_t dir_entry;
    bool found = dir_lookup_name(this, name, &dir_entry);
    if (!found)
        return NULL;
    return fs_seek_inode(dir_entry.ino);
}

int dir_remove_entry(inode_t *this, char *name)
{
    int ino = FAKE_INO;
    dir_entry_t *dir_entry;
    dir_cursor_t dir_cursor;

    dir_cursor_init(&dir_cursor, this);
    while (dir_entry = dir_cursor_next(&dir_cursor)) {
        if (dir_entry->ino == FAKE_INO)
            continue;
        if (strcmp(dir_entry->name, name) == 0) {
            ino = dir_entry->ino;
            dir_entry_reset(dir_entry);
            break;
        }
    }
    dir_cursor_destroy(&dir_cursor);
    return ino;
}

bool dir_reuse_entry(inode_t *this, char *name, int ino)
{
    bool found = 0;
    dir_cursor_t dir_cursor;
    dir_entry_t *dir_entry;

    dir_cursor_init(&dir_cursor, this);
    while (dir_entry = dir_cursor_next(&dir_cursor)) {
        if (dir_entry->ino == FAKE_INO) {
            found = 1;
            dir_entry_set(dir_entry, name, ino);
            break;
        }
    }
    dir_cursor_destroy(&dir_cursor);
    return found;
}

void dir_add_dot(inode_t *this, int this_ino, int parent_ino)
{
    int bno = fs_alloc_bno();
    this->address[0] = bno;
    
    dir_entry_t *dir_entry = disk_seek_block(bno);
    dir_entry_set(dir_entry + 0, ".", this_ino);
    dir_entry_set(dir_entry + 1, "..", parent_ino);

    this->size = sizeof(dir_entry_t) * 2;
    this->link_count = 2;
}

int dir_add_entry(inode_t *this, char *name, int ino)
{
    dir_entry_t dir_entry;
    if (dir_lookup_name(this, name, &dir_entry))
        return -EEXIST;

    if (dir_reuse_entry(this, name, ino))
        return 0;

    dir_entry_set(&dir_entry, name, ino);
    inode_write(this, this->size, &dir_entry, sizeof(dir_entry_t));
    this->link_count += 1;
    return 0;
}

int dir_make_common(inode_t *this, char *name, int mode, inode_t **result)
{
    int ino = fs_alloc_ino();
    if (ino == FAKE_INO)
        return -ENOSPC;

    int error = dir_add_entry(this, name, ino);
    if (error < 0) {
        fs_free_ino(ino);
        return error;
    }

    this->link_count += 1;
    inode_t *leaf = fs_seek_inode(ino);
    inode_init(leaf, mode);
    *result = leaf;
    return 0;
}

int dir_make_regular(inode_t *this, char *name, inode_t **result)
{
    int mode = S_IFREG | 0664;
    int error = dir_make_common(this, name, mode, result);
    return error;
}

int dir_make_dir(inode_t *this, char *name, inode_t **result)
{
    int mode = S_IFDIR | 0775;
    inode_t *leaf;
    int error = dir_make_common(this, name, mode, &leaf);
    if (error < 0)
        return error;

    int leaf_ino = fs_seek_ino(leaf);
    int this_ino = fs_seek_ino(this);
    dir_add_dot(leaf, leaf_ino, this_ino);
    *result = leaf;
    return 0;
}

int namei(int flag, const char *path_read_only, inode_t **ip, char *base_name)
{
    char path[PATH_MAX];
    strcpy(path, path_read_only);

    inode_t *parent;
    if (path[0] == '/') 
        parent = root_dir;
    else 
        parent = work_dir;
    inode_t *child = NULL;

    int error = 0;
    char *word_table[128];
    int word_count = split_string(path, "/", word_table);

    for (int i = 0; i < word_count; i++) {
        if (!S_ISDIR(parent->mode)) {
            error = -ENOTDIR;
            goto bad;
        }

        char *word = word_table[i];
        if (i == word_count - 1 && flag == NAMEI_MAKE) {
            *ip = parent;
            strcpy(base_name, word);
            return 0;
        }

        child = dir_lookup_inode(parent, word);
        if (!child) {
            error = -ENOENT;
            goto bad;
        }

        parent = child;
    }

    *ip = parent;
    return 0;

bad:
    *ip = NULL;
    return error;
}

int namei_open(const char *path, inode_t **ip)
{
    return namei(NAMEI_OPEN, path, ip, NULL);
}

int namei_make(const char *path, inode_t **ip, char *base_name)
{
    return namei(NAMEI_MAKE, path, ip, base_name);
}

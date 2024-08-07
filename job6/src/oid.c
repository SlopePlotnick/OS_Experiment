#include "utils/std.h"
#include "utils/file.h"
#include "utils/dir.h"
#include "config.h"
#include "oid.h"
#include <openssl/sha.h>

void oid_set(oid_t *this, char *data)
{
    strcpy(this->data, data);
}

bool oid_is_zero(oid_t *this)
{
    return strcmp(this->data, OID_ZERO) == 0;
}

bool oid_equal(oid_t *this, oid_t *that)
{
    return strcmp(this->data, that->data) == 0;
}

void get_file_digest(char *path, unsigned char digest[20])
{
    int fd = open(path, O_RDONLY);
    assert(fd > 0);

    SHA_CTX ctx;
    SHA1_Init(&ctx);

    char buff[4096];
    int count;
    while (count = read(fd, buff, sizeof(buff)))
        SHA1_Update(&ctx, buff, count);

    SHA1_Final(digest, &ctx);
    close(fd);
}

void digest_to_oid(unsigned char digest[20], oid_t *oid)
{
    char *p = oid->data;

    int i;
    for (i = 0; i < 20; i++) {
        sprintf(p, "%02x", digest[i]);
        p += 2;
    }
    *p = 0;
}

void get_string_oid(char *string, oid_t *oid)
{
    unsigned char digest[20];
    int length = strlen(string);
    SHA1(string, length, digest);

    digest_to_oid(digest, oid);
}

void get_oid_path(oid_t *oid, char *path)
{
    sprintf(path, "%s/%s", GIT_OBJECTS_DIR, oid->data);
}

bool oid_exists(oid_t *this)
{
    char path[PATH_MAX];
    get_oid_path(this, path);

    return file_exists(path);
}

void get_file_oid(char *path, oid_t *oid)
{
    unsigned char digest[20];
    get_file_digest(path, digest);

    digest_to_oid(digest, oid);
}

FILE *fopen_oid(oid_t *oid, char *mode)
{
    char path[PATH_MAX];
    get_oid_path(oid, path);

    FILE *f = fopen(path, mode);
    return f;
}

static bool starts_with(char *full, char *abbr)
{
    int full_len = strlen(full);
    int abbr_len = strlen(abbr);
    assert(full_len == 40);
    assert(abbr_len < 40);

    return memcmp(full, abbr, abbr_len) == 0;
}

void make_oid(char *abbr, oid_t *oid)
{
    int count = 0;

    dir_t *dir = dir_open(GIT_OBJECTS_DIR);
    entry_t entry;

    while (dir_read(dir, &entry)) {
        if (starts_with(entry.name, abbr)) {
            count++;
            oid_set(oid, entry.name);
        }
    }
    dir_close(dir);

    if (count != 1)
        fatal("found %d object oids that match %s", count, oid);
}

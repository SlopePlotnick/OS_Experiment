#include "utils/std.h"
#include <sys/mman.h>
#include "disk.h"
#include "fs.h"

disk_t disk;

void disk_create(char *path, int size)
{
    mode_t mode = 0664;
    int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, mode);
    if (fd < 0)
        panic("open");

    int error;

    error = lseek(fd, size - 1, SEEK_SET);
    if (error < 0)
        panic("lseek");

    error = write(fd, "\0", 1);
    if (error != 1)
        panic("write");

    close(fd);
}

void *disk_open(char *path)
{
    int fd = open(path, O_RDWR);
    if (fd < 0)
        panic("open");

    struct stat stat;
    int error = fstat(fd, &stat);
    if (error < 0)
        panic("fstat");

    int size = stat.st_size;
    void *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
        panic("mmap");

    disk.fd = fd;
    disk.data = data;
    disk.size = size;
    return data;
}

void disk_close()
{
    munmap(disk.data, disk.size);
    close(disk.fd);
}

void *disk_seek_byte(int pos)
{
    return disk.data + pos;
}

void *disk_seek_block(int bno)
{
    return disk.data + bno * BLOCK_SIZE;
}



#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() 
{
    void *addr = (void *) (8 * 0x1000 * 0x1000);
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE | MAP_ANON | MAP_FIXED;
    void *map = mmap(addr, 4096, prot, flags, -1, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    assert(map == addr);

    strcpy(map, "hello");
    puts(map);

    munmap(map, 4096);
    return 0;
}

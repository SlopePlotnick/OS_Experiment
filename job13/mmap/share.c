#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/mman.h>

#define FILE_SIZE 4096

void panic(char *message) 
{
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int fd = open("page.txt", O_RDWR);
    if (fd < 0)
        panic("open");

    char *p = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED)
        panic("mmap");

    int i;
    for (i = 0; i < FILE_SIZE; i++) {
        if (islower(p[i]))
            p[i] = toupper(p[i]);
    }

    munmap(p, FILE_SIZE);
    close(fd);
    return 0;
}

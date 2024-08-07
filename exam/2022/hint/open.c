#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd;
    fd = open("log", O_RDONLY);

    char buf[512];
    int count = read(fd, buf, sizeof(buf));
    buf[count] = 0;
    puts(buf);

    close(fd);
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd;
    fd = open("log", O_CREAT|O_WRONLY, 0666); 
    write(fd, "hello\n", 6);  
    close(fd);
    return 0;
}

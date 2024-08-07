#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int from = open(argv[1], O_RDONLY);
    int to = creat(argv[2], S_IRWXU);

    char ch;
    while (read(from, &ch, sizeof(ch)))
    {
        write(to, &ch, sizeof(ch));
    }

    close(from);
    close(to);

    return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#define CONSUMERS 2
#define ITEMS 10

void delay()
{
    int i;
    for (i = 0; i < 87654321; i++)
        ;
}

void consume(int fd)
{
    while (1) {
        char buf;
        int count = read(fd, &buf, 1);
        assert(count == 1);
        delay();

        if (buf == 'z')
            break;
        buf = toupper(buf);
        printf("%d: %c\n", getpid(), buf);
    }
}

void produce(int fd)
{
    int i;
    for (i = 0; i < ITEMS; i++) {
        char buf = 'a' + i;
        int count = write(fd, &buf, 1);
        assert(count == 1);
        delay();
    }

    for (i = 0; i < CONSUMERS; i++) {
        char buf = 'z';
        write(fd, &buf, 1);
    }
}

int main()
{
    int fd_array[2];
    pipe(fd_array);

    int i;
    for (i = 0; i < CONSUMERS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            consume(fd_array[0]);
            exit(0);
        }
    }

    produce(fd_array[1]);
    puts("END");
    return 0;
}

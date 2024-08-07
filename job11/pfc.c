#include <stdio.h>
#include <stdlib.h>
#include "lib/coro.h"

coro_t *coro_p;
coro_t *coro_f;
coro_t *coro_c;

void produce()
{
    puts("PRODUCE");

    printf("produce 0\n");
    coro_yield(0);

    printf("produce 1\n");
    coro_yield(1);

    printf("produce 2\n");
    coro_yield(2);

    printf("produce 3\n");
    coro_yield(3);

    printf("produce 4\n");
    coro_yield(4);

    printf("produce 5\n");
    coro_yield(5);

    printf("produce 6\n");
    coro_yield(6);

    printf("produce 7\n");
    coro_yield(7);

    puts("NEVER REACH HERE");
}

void filter()
{
    int x, new_x;

    puts("\tFILTER");

    x = coro_resume(coro_p);
    new_x = x * 10;
    printf("\tfilter %d -> %d\n", x, new_x);
    coro_yield(new_x);

    x = coro_resume(coro_p);
    new_x = x * 10;
    printf("\tfilter %d -> %d\n", x, new_x);
    coro_yield(new_x);

    x = coro_resume(coro_p);
    new_x = x * 10;
    printf("\tfilter %d -> %d\n", x, new_x);
    coro_yield(new_x);

    x = coro_resume(coro_p);
    new_x = x * 10;
    printf("\tfilter %d -> %d\n", x, new_x);
    coro_yield(new_x);

    x = coro_resume(coro_p);
    new_x = x * 10;
    printf("\tfilter %d -> %d\n", x, new_x);
    coro_yield(new_x);

    x = coro_resume(coro_p);
    new_x = x * 10;
    printf("\tfilter %d -> %d\n", x, new_x);
    coro_yield(new_x);

    x = coro_resume(coro_p);
    new_x = x * 10;
    printf("\tfilter %d -> %d\n", x, new_x);
    coro_yield(new_x);

    x = coro_resume(coro_p);
    new_x = x * 10;
    printf("\tfilter %d -> %d\n", x, new_x);
    coro_yield(new_x);
}

void consume()
{
    int x;

    puts("\t\tCONSUME");

    x = coro_resume(coro_f);
    printf("\t\tconsume %d\n", x);

    x = coro_resume(coro_f);
    printf("\t\tconsume %d\n", x);

    x = coro_resume(coro_f);
    printf("\t\tconsume %d\n", x);

    x = coro_resume(coro_f);
    printf("\t\tconsume %d\n", x);

    x = coro_resume(coro_f);
    printf("\t\tconsume %d\n", x);

    x = coro_resume(coro_f);
    printf("\t\tconsume %d\n", x);

    x = coro_resume(coro_f);
    printf("\t\tconsume %d\n", x);

    x = coro_resume(coro_f);
    printf("\t\tconsume %d\n", x);

    puts("END");
    exit(0);
}

int main()
{
    coro_p = coro_new(produce);
    coro_f = coro_new(filter);
    coro_c = coro_new(consume);
    coro_boot(coro_c);
    return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>

#define CAPACITY 4
#define ITEM_COUNT 5

struct buff
{
    int data[CAPACITY];
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

struct buff b0, b1;

int buff_is_empty(struct buff *buff)
{
    return buff->in == buff->out;
}

int buff_is_full(struct buff *buff)
{
    return (buff->in + 1) % CAPACITY == buff->out;
}

void buff_init(struct buff *buff)
{
    buff->in = 0;
    buff->out = 0;
    pthread_mutex_init(&buff->mutex, NULL);
    pthread_cond_init(&buff->cond, NULL);
}

// 可能会阻塞
int buff_get_item(struct buff *buff)
{
    pthread_mutex_lock(&buff->mutex);
    while (buff_is_empty(buff))
        pthread_cond_wait(&buff->cond, &buff->mutex);

    int item = buff->data[buff->out];
    buff->out = (buff->out + 1) % CAPACITY;

    pthread_cond_broadcast(&buff->cond);
    pthread_mutex_unlock(&buff->mutex);
    return item;
}

// 可能会阻塞
void buff_put_item(struct buff *buff, int item)
{
    pthread_mutex_lock(&buff->mutex);
    while (buff_is_full(buff))
        pthread_cond_wait(&buff->cond, &buff->mutex);

    buff->data[buff->in] = item;
    buff->in = (buff->in + 1) % CAPACITY;

    pthread_cond_broadcast(&buff->cond);
    pthread_mutex_unlock(&buff->mutex);
}

void *ping(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++)
    {
        item = 'a' + i;
        buff_put_item(&b0, item);
        printf("ping: SEND %c\n", item);

        item = buff_get_item(&b1);
        printf("ping: RECV %c\n", item);
    }

    return NULL;
}

void *pong(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++)
    {
        item = buff_get_item(&b0);
        printf("    pong: RECV %c\n", item);
        item = toupper(item);
        buff_put_item(&b1, item);
        printf("    pong: SEND %c\n", item);
    }

    return NULL;
}

int main()
{
    buff_init(&b0);
    buff_init(&b1);

    pthread_t ping_tid;
    pthread_create(&ping_tid, NULL, ping, NULL);

    pthread_t pong_tid;
    pthread_create(&pong_tid, NULL, pong, NULL);

    pthread_join(ping_tid, NULL);
    pthread_join(pong_tid, NULL);
    return 0;
}
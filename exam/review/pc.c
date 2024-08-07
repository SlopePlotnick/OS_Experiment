#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

/*
producer  computer  consumer
        b1        b2
*/

#define CAPACITY 4
#define ITEM_COUNT 8

struct buff {
    int data[CAPACITY];
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

struct buff b1, b2;

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

void *produce(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
        item = 'a' + i;
        buff_put_item(&b1, item);
        printf("produce item: %c\n", item);
    }
    return NULL;
}

void *compute(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
        item = buff_get_item(&b1);

        item = toupper(item);
        printf("  compute item: %c\n", item);

        buff_put_item(&b2, item);
    }
    return NULL;
}

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
        item = buff_get_item(&b2);
        printf("    consume item: %c\n", item);
    }
    return NULL;
}

int main()
{
    buff_init(&b1);
    buff_init(&b2);

    pthread_t computer_tid;
    pthread_create(&computer_tid, NULL, compute, NULL);

    pthread_t consumer_tid;
    pthread_create(&consumer_tid, NULL, consume, NULL);

    produce(NULL);
    pthread_join(computer_tid, NULL);
    pthread_join(consumer_tid, NULL);
    return 0;
}


#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

void sema_init(sema_t *sema, int value)
{
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    ++sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

#define CAPACITY 3

typedef struct {
    int data[CAPACITY];
    int in;             // 缓冲区的写指针
    int out;            // 缓冲区的读指针
    sema_t mutex;
    sema_t empty_buff_sema;
    sema_t full_buff_sema;
} buff_t;

void buff_init(buff_t *buff)
{
    buff->in = 0;
    buff->out = 0;
    sema_init(&buff->mutex, 1);
    sema_init(&buff->empty_buff_sema, CAPACITY);
    sema_init(&buff->full_buff_sema, 0);
}

void buff_put(buff_t *buff, int item)
{
    sema_wait(&buff->empty_buff_sema);
    sema_wait(&buff->mutex);
    buff->data[buff->in] = item;
    buff->in = (buff->in + 1) % CAPACITY;
    sema_signal(&buff->mutex);
    sema_signal(&buff->full_buff_sema);
}

int buff_get(buff_t *buff)
{
    sema_wait(&buff->full_buff_sema);
    sema_wait(&buff->mutex);
    int item = buff->data[buff->out];
    buff->out = (buff->out + 1) % CAPACITY;
    sema_signal(&buff->mutex);
    sema_signal(&buff->empty_buff_sema);
    return item;
}

typedef struct {
    buff_t *in;
    buff_t *out;
} param_t;

void *ping_entry(void *arg)
{
    param_t *param = (param_t *)arg;

    int i;
    for (i = 0; i < 5; i++) {
        int item;

        item = 'a' + i;
        buff_put(param->out, item);
        printf("ping: SEND %c\n", item);

        item = buff_get(param->in);
        printf("ping: RECV %c\n", item);
    }
    return NULL;
}

void *pong_entry(void *arg)
{
    param_t *param = (param_t *)arg;

    int i;
    for (i = 0; i < 5; i++) {
        int item;

        item = buff_get(param->in);
        printf("    pong: RECV %c\n", item);
        item = toupper(item);
        buff_put(param->out, item);
    }
    return NULL;
}

int main()
{
    pthread_t ping;
    pthread_t pong;
    
    buff_t b0, b1;
    buff_init(&b0);
    buff_init(&b1);

    param_t ping_param = {&b1, &b0};
    param_t pong_param = {&b0, &b1};

    pthread_create(&ping, NULL, ping_entry, &ping_param);
    pthread_create(&pong, NULL, pong_entry, &pong_param);

    pthread_join(ping, NULL);
    pthread_join(pong, NULL);
    return 0;
}

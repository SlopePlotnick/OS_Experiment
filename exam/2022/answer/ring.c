#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

#define CAPACITY 4      // 缓冲区的最大容量

typedef struct {
    int data[CAPACITY];
    int in;             // 缓冲区的写指针
    int out;            // 缓冲区的读指针
    pthread_mutex_t mutex;
    pthread_cond_t wait_empty_buffer;
    pthread_cond_t wait_full_buffer;
} buffer_t;

// 初始化 buffer，需要初始化用于同步的字段 
void buffer_init(buffer_t *buffer)
{
    buffer->in = 0;
    buffer->out = 0;

    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->wait_empty_buffer, NULL);
    pthread_cond_init(&buffer->wait_full_buffer, NULL);
}

// 判断缓冲区是否为空
int buffer_is_empty(buffer_t *buffer)
{
    return buffer->in == buffer->out;
}

// 判断缓冲区是否为满
int buffer_is_full(buffer_t *buffer)
{
    return (buffer->in + 1) % CAPACITY == buffer->out;
}

// 向缓冲区中追加一个数据，如果缓冲区为满，则等待
void buffer_put(buffer_t *buffer, int item)
{
    pthread_mutex_lock(&buffer->mutex);
    while (buffer_is_full(buffer))
        pthread_cond_wait(&buffer->wait_empty_buffer, &buffer->mutex);

    buffer->data[buffer->in] = item;
    buffer->in = (buffer->in + 1) % CAPACITY;

    pthread_cond_signal(&buffer->wait_full_buffer);
    pthread_mutex_unlock(&buffer->mutex);
}

// 从缓冲区中取走一个数据，如果缓冲区为空，则等待
int buffer_get(buffer_t *buffer)
{
    pthread_mutex_lock(&buffer->mutex);
    while (buffer_is_empty(buffer))
        pthread_cond_wait(&buffer->wait_full_buffer, &buffer->mutex);

    int item = buffer->data[buffer->out];
    buffer->out = (buffer->out + 1) % CAPACITY;

    pthread_cond_signal(&buffer->wait_empty_buffer);
    pthread_mutex_unlock(&buffer->mutex);
    return item;
}

typedef struct {
    int id;
    buffer_t *in;
    buffer_t *out;
} param_t;

void *thread_entry(void *arg)
{
    param_t *param = arg;
    int i;
    for (i = 0; i < 3; i++) {
        int item = buffer_get(param->in);
        printf("T%d: %d\n", param->id, item);
        buffer_put(param->out, item + 1);
    }
    return NULL;
}

#define N 3

int main()
{
    buffer_t buffers[N];
    pthread_t pthreads[N];
    param_t params[N];

    int i;
    for (i = 0; i < N; i++) {
        buffer_t *buffer = buffers + i;
        buffer_init(buffer);
    }

    for (i = 0; i < N; i++) {
        int j = (i + 1) % N;
        buffer_t *in = buffers + i;
        buffer_t *out = buffers + j;
        param_t *param = params + i;
        param->id = i;
        param->in = in;
        param->out = out;
    }

    for (i = 0; i < N; i++) {
        param_t *param = params + i;
        pthread_t *pthread = pthreads + i;
        pthread_create(pthread, NULL, thread_entry, param);
    }

    buffer_put(&buffers[0], 0);

    for (i = 0; i < N; i++) {
        pthread_t pthread = pthreads[i];
        pthread_join(pthread, NULL);
    } 

    puts("END");
    return 0;
}

// sno: input your student no
// name: input your name (pin yin), ex. zhang san
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

#define CAPACITY 4

typedef struct
{
    int data[CAPACITY];
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t wait_empty_buffer;
    pthread_cond_t wait_full_buffer;
} buffer_t;

void buffer_init(buffer_t *buffer)
{
    buffer->in = 0;
    buffer->out = 0;

    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->wait_empty_buffer, NULL);
    pthread_cond_init(&buffer->wait_full_buffer, NULL);
}

int buffer_is_empty(buffer_t *buffer)
{
    return buffer->in == buffer->out;
}

int buffer_is_full(buffer_t *buffer)
{
    return (buffer->in + 1) % CAPACITY == buffer->out;
}

// Todo
void buffer_put(buffer_t *buffer, int item)
{
    pthread_mutex_lock(&(buffer->mutex));
    while (buffer_is_full(buffer))
        pthread_cond_wait(&(buffer->wait_empty_buffer), &(buffer->mutex));

    buffer->data[buffer->in] = item;
    buffer->in = (buffer->in + 1) % CAPACITY;

    pthread_cond_signal(&buffer->wait_full_buffer);
    pthread_mutex_unlock(&buffer->mutex);
}

// Todo
int buffer_get(buffer_t *buffer)
{
    pthread_mutex_lock(&buffer->mutex);
    while (buffer_is_empty(buffer))
        pthread_cond_wait(&buffer->wait_full_buffer, &buffer->mutex);

    int item;

    item = buffer->data[buffer->out];
    buffer->out = (buffer->out + 1) % CAPACITY;

    pthread_cond_signal(&buffer->wait_empty_buffer);
    pthread_mutex_unlock(&buffer->mutex);

    return item;
}

typedef struct param_t
{
    int id;
    buffer_t *in;
    buffer_t *out;
} param_t;

void *compute(void *arg)
{
    param_t *param;
    param = (param_t *)arg;
    for (int i = 0; i < 3; i++)
    {
        int item;
        item = buffer_get(param->in);
        printf("T%d: %d\n", param->id, item);
        item = item + 1;
        buffer_put(param->out, item);
    }
}

#define N 3

int main()
{
    buffer_t buffer[3];
    for (int i = 0; i < 3; i++)
    {
        buffer_init(&(buffer[i]));
    }

    pthread_t pthreads[3];
    param_t params[3];
    buffer_put(&buffer[0], 0);
    for (int i = 0; i < 3; i++)
    {
        param_t *param;
        param = &params[i];
        param->id = i;
        param->in = &buffer[i];
        param->out = &buffer[(i + 1) % 3];
        pthread_create(&pthreads[i], NULL, compute, param);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(pthreads[i], NULL);
    }

    puts("END");
    return 0;
}
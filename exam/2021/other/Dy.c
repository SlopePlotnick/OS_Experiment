// + 补全生产者、消费者程序
//   - 完成函数 buffer_init
//   - 完成函数 buffer_get
//   - 完成函数 buffer_put
//   - 补全函数 main
//     * 创建生产者、消费者
//
// + 程序最终输出
//   - 生产者输出 a b c d e f g h
//   - 消费者输出 a b c d e f g h
//   - 两者交叉输出 
//
// + 不准对函数 produce 和 函数 consume 进行任何改动
//   - 如果改动，则该题没有分数

#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

#define CAPACITY 4      // 缓冲区的最大容量

struct buffer {
    int data[CAPACITY];
    int in;             // 缓冲区的写指针
    int out;            // 缓冲区的读指针
    pthread_mutex_t mutex;
    pthread_cond_t wait_empty_buffer;
    pthread_cond_t wait_full_buffer;
};

// 初始化 buffer，需要初始化用于同步的字段
void buffer_init(struct buffer *buffer)
{
    buffer->in = 0;
    buffer->out = 0;
}

// 判断缓冲区是否为空
int buffer_is_empty(struct buffer *buffer)
{
    return buffer->in == buffer->out;
}

// 判断缓冲区是否为满
int buffer_is_full(struct buffer *buffer)
{
    return (buffer->in + 1) % CAPACITY == buffer->out;
}

// 向缓冲区中追加一个数据，如果缓冲区为满，则等待
void buffer_put(struct buffer *buffer, int item)
{
}

// 从缓冲区中取走一个数据，如果缓冲区为空，则等待
int buffer_get(struct buffer *buffer)
{
}

#define ITEM_COUNT (2 * CAPACITY)

struct buffer buffer;

void *produce(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        item = 'a' + i;
        printf("produce item: %c\n", item); 
        buffer_put(&buffer, item);
    }
    return NULL;
}

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        item = buffer_get(&buffer); 
        printf("        consume item: %c\n", item); 
    }
    return NULL;
}

int main()
{ 
    buffer_init(&buffer);

    return 0;
}

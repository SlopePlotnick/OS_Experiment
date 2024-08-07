#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#define N 1000

typedef struct
{
    char path[N];
    char target[N];
} task;

task buffer[N];
int in;
int out;

int buffer_is_empty()
{
    return in == out;
}

int buffer_is_full()
{
    return (in + 1) % N == out;
}

task get_item()
{
    task item;

    item = buffer[out];
    out = (out + 1) % N;
    return item;
}

void put_item(task item)
{
    buffer[in] = item;
    in = (in + 1) % N;
}

typedef struct
{
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

sema_t mutex_sema;
sema_t empty_buffer_sema;
sema_t full_buffer_sema;

void grep_file(char *path, char *target)
{
    FILE *file = fopen(path, "r");

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, target))
            printf("%s:%s", path, line);
    }

    fclose(file);
}

void grep_dir(char *path, char *target)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    while (entry = readdir(dir))
    {
        if (strcmp(entry->d_name, ".") == 0)
            continue;

        if (strcmp(entry->d_name, "..") == 0)
            continue;

        char sub_path[300];
        sprintf(sub_path, "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR)
            grep_dir(sub_path, target);

        if (entry->d_type == DT_REG)
            grep_file(sub_path, target);
    }
    closedir(dir);
}

void *consume(void *arg)
{
    task item;

    while (1)
    {
        sema_wait(&full_buffer_sema);
        sema_wait(&mutex_sema);

        item = get_item();
        // printf("get item path=%s\n", item.path);

        sema_signal(&mutex_sema);
        sema_signal(&empty_buffer_sema);

        if (!strcmp(item.path, "\0") && !strcmp(item.target, "\0"))
            pthread_exit(NULL);
        else
            grep_file(item.path, item.target);
    }

    return NULL;
}

void produce_file(char *path, char *target)
{
    task item;
    strcpy(item.path, path);
    strcpy(item.target, target);

    sema_wait(&empty_buffer_sema);
    sema_wait(&mutex_sema);

    put_item(item);

    sema_signal(&mutex_sema);
    sema_signal(&full_buffer_sema);
}

void produce_dir(char *path, char *target)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    while (entry = readdir(dir))
    {
        if (strcmp(entry->d_name, ".") == 0)
            continue;

        if (strcmp(entry->d_name, "..") == 0)
            continue;

        char sub_path[300];
        sprintf(sub_path, "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR)
            produce_dir(sub_path, target);

        if (entry->d_type == DT_REG)
            produce_file(sub_path, target);
    }
    closedir(dir);
}

void usage()
{
    puts("Usage:");
    puts(" pgrep string path");
    puts(" pgrep -r string path");
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        usage();
        exit(0);
    }

    char *string = argv[argc - 2];
    char *path = argv[argc - 1];

    pthread_t tid[2];
    sema_init(&mutex_sema, 1);
    sema_init(&empty_buffer_sema, N - 1);
    sema_init(&full_buffer_sema, 0);

    for (int i = 0; i < 2; i++)
    {
        pthread_create(&tid[i], NULL, consume, NULL);
    }
    if (strcmp(argv[1], "-r") == 0)
        produce_dir(path, string);
    else
        produce_file(path, string);
    // 主线程搜索完后给两个结束标志
    for (int i = 0; i < 2; i++)
    {
        task item;
        strcpy(item.path, "\0");
        strcpy(item.target, "\0");
        sema_wait(&empty_buffer_sema);
        sema_wait(&mutex_sema);
        put_item(item);
        sema_signal(&mutex_sema);
        sema_signal(&full_buffer_sema);
    }
    for (int i = 0; i < 2; i++)
    {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
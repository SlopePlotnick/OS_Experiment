#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

const int N = 1e4;

typedef struct param_t
{
    int start;
    int end;
} param_t;

typedef struct result_t
{
    double sum;
} result_t;

void *compute(void *arg)
{
    param_t *param;
    result_t *result;
    double sum;

    param = (param_t *)arg;
    for (int i = param->start; i <= param->end; i++)
    {
        sum += (pow(-1, i) / (2 * i + 1));
    }

    result = malloc(sizeof(result_t));
    result->sum = sum;
    return result;
}

int main()
{
    // 创建并执行子线程
    pthread_t son;
    param_t *param = malloc(sizeof(param_t));

    param->start = N / 2;
    param->end = N - 1;
    pthread_create(&son, NULL, compute, param);

    // 等待子线程计算完毕 在主线程中计算前半部分
    double sum = 0;
    result_t *sum2 = malloc(sizeof(result_t));
    pthread_join(son, (void **)&sum2);
    double sum1 = 0;
    for (int i = 0; i <= N / 2 - 1; i++)
    {
        sum1 += (pow(-1, i) / (2 * i + 1));
    }
    sum += sum1 + sum2->sum;

    printf("%.2lf\n", sum * 4);

    return 0;
}
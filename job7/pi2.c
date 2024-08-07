#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

const int NR_TOTAL = 1e4;
const int NR_CPU = 8;
const int NR_CHILD = NR_TOTAL / NR_CPU;

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
    for (int i = param->start; i < param->end; i++)
    {
        sum += (pow(-1, i) / (2 * i + 1));
    }

    result = malloc(sizeof(result_t));
    result->sum = sum;
    return result;
}

int main()
{
    pthread_t workers[NR_CPU];
    param_t params[NR_CPU];

    for (int i = 0; i < NR_CPU; i++)
    {
        param_t *param;
        param = &params[i];
        param->start = i * NR_CHILD;
        param->end = (i + 1) * NR_CHILD;
        pthread_create(&workers[i], NULL, compute, param);
    }

    double sum = 0;
    for (int i = 0; i < NR_CPU; i++)
    {
        result_t *result;
        pthread_join(workers[i], (void **)&result);
        sum += result->sum;
        free(result);
    }

    printf("%.2lf\n", sum * 4);

    return 0;
}
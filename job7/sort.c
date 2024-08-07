#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

const int N = 1e4;

const int NR_TOTAL = 8;
const int NR_CPU = 2;
const int NR_CHILD = NR_TOTAL / NR_CPU;

typedef struct param_t
{
    int *q;
    int n;
} param_t;

void swap(int *a, int *b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

// 选择排序
void *select_sort(void *arg)
{
    param_t *param  = (param_t *)arg;

    int index; // 存储搜索到的最小元素的下标
    int myMin; // 记录搜索到的最小值
    for (int i = 0; i < (param->n) - 1; i++)
    {
        index = i; // 假设最小值为第一个数
        myMin = (param->q)[i];
        // i表示的是当前应从哪个数开始往后搜索
        for (int j = i + 1; j < (param->n); j++)
        {
            if ((param->q)[j] < myMin)
            {
                index = j;
                myMin = (param->q)[j];
            }
        }
        if (index != i)
            swap(&((param->q)[i]), &((param->q)[index]));
    }
}

//归并排序
void merge_sort(int q[], int l, int r){
    if (l >= r) return;
    
    int mid = (l + r) / 2;
    
    merge_sort(q, l, mid), merge_sort(q, mid + 1, r);
    
    int tmp[N], k = 0, i = l, j = mid + 1;
    while (i <= mid && j <= r){
        if (q[i] <= q[j]) tmp[k++] = q[i++];
        else tmp[k++] = q[j++];
    }
    while (i <= mid) tmp[k++] = q[i++];
    while (j <= r) tmp[k++] = q[j++];
    
    for (int i = l, j = 0; i <= r; i++, j++){
        q[i] = tmp[j];
    }
}

void parallel_sort(int *array, int size)
{
    pthread_t workers[NR_CPU];
    param_t params[NR_CPU];

    for (int i = 0; i < NR_CPU; i++)
    {
        param_t *param;
        param = &params[i];
        param->q = array + i * NR_CHILD;
        param->n = NR_CHILD;
        pthread_create(&workers[i], NULL, select_sort, param);
    }

    for (int i = 0; i < NR_CPU; i++)
    {
        pthread_join(workers[i], NULL);
    }

    merge_sort(array, 0, 8 - 1);
}

int main()
{
    int array[8] = {3, 1, 2, 4, 5, 6, 7, 0};
    parallel_sort(array, 8);
    int i;
    for (i = 0; i < 8; i++)
        printf("%d\n", array[i]);
    return 0;
}
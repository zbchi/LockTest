#include "Lock.h"
pthread_mutex_t mutex;
int number;

// 线程处理函数
void* funcA_num(void* arg)
{
    for(int i=0; i<MAX; ++i)
    {
        pthread_mutex_lock(&mutex);
        number++;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* funcB_num(void* arg)
{
    for(int i=0; i<MAX; ++i)
    {
        pthread_mutex_lock(&mutex);
        number++;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}


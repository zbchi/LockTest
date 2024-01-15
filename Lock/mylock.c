#include "Lock.h"
pthread_mutex_t mutex;
pthread_mutex_t mutex1;
int number;

// 线程处理函数
void* funcA_num(void* arg)
{
    for(int i=0; i<MAX; ++i)
    {
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex1);
        number++;
        sleep(1);
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* funcB_num(void* arg)
{
    for(int i=0; i<MAX; ++i)
    {
        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex);
        number++;
        sleep(1);
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&mutex1);
    }

    return NULL;
}


#ifndef _LOCK_H_
#define _LOCK_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

// g++ mylock.c  test.cpp -lgtest -lgtest_main -lpthread -fsanitize=thread  -g
#define MAX 100
// pthread_mutex_t mutex;

void* funcA_num(void* arg);
void* funcB_num(void* arg);
#endif
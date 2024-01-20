#include "lock_test.h"

#include <stdio.h>

void LockTestInit(lock_test_t* mutex) {
  if(pthread_mutex_init(&mutex->lock,NULL) != 0) {
    printf("init failed...");
    return;
  }
}

void Deposit(lock_test_t mutex,int balance,int amount) {
  pthread_mutex_lock(&mutex.lock);
  balance += amount;
  pthread_mutex_unlock(&mutex.lock);
}

void Withdraw(lock_test_t mutex, int balance, int amount) {
  pthread_mutex_lock(&mutex.lock);
  balance -= amount;
  pthread_mutex_unlock(&mutex.lock);
}

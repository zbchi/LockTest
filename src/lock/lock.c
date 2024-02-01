#include "lock.h"

void amountInit(lock_t* Account) {
  if(pthread_mutex_init(&Account->mutex,NULL) != 0) {
    return;
  }
  Account->amount = 0;
}

void Income(lock_t* Account, int amount) {
  pthread_mutex_lock(&Account->mutex);
  Account->amount += amount;
  pthread_mutex_unlock(&Account->mutex);
}

void Expend(lock_t* Account, int amount) {
  pthread_mutex_lock(&Account->mutex);
  Account->amount -= amount;
  pthread_mutex_unlock(&Account->mutex);
}
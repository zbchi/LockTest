#include "lock.h"

void amountInit(lock_t* mutex) {
  if(pthread_mutex_init(&mutex->lock,NULL) != 0) {
    return;
  }
}

void Income(lock_t Account, int amount) {
  pthread_mutex_lock(&Account.lock);
  Account.amount += amount;
  pthread_mutex_unlock(&Account.lock);
}

void Expend(lock_t Accout, int amount) {
  pthread_mutex_lock(&Accout.lock);
  Accout.amount -= amount;
  pthread_mutex_unlock(&Accout.lock);
}

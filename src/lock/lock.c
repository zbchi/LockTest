#include "lock.h"

#include <stdio.h>

void amountInit(lock_t* Account) {
  pthread_mutex_lock(&(Account->mutex));
  Account->amount=0;
  pthread_mutex_unlock(&(Account->mutex));

}

void Income(lock_t* Account, int amount) {
  pthread_mutex_lock(&(Account->mutex));
  Account->amount+=amount;
  pthread_mutex_unlock(&(Account->mutex));

}

void Expend(lock_t* Account, int amount) {
  pthread_mutex_lock(&(Account->mutex));
  Account->amount-=amount;
  pthread_mutex_unlock(&(Account->mutex));

}
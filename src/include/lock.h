#include <pthread.h>

typedef struct {
  int amount;
  pthread_mutex_t lock;
}lock_t;

void amountInit(lock_t* mutex);
void Income(lock_t* mutex, int amount);
void Expend(lock_t* mutex, int amount);
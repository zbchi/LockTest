#include <pthread.h>

void AddNumber(int number);

typedef struct {
  pthread_mutex_t lock;
}lock_test_t;

void LockTestInit(lock_test_t* mutex);
void Deposit(lock_test_t mutex, int balance, int amount);
void Withdraw(lock_test_t mutex, int balance, int amount);
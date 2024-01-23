//#include <gtest/gtest.h>
#include <pthread.h>

typedef struct {
  int amount;
  pthread_mutex_t lock;
}lock_t;

void amountInit(lock_t* mutex);
void Income(lock_t mutex, int amount);
void Expend(lock_t mutex, int amount);

class LockTest : public ::testing::Test {
 protected:
  virtual void SetUp() { 
    amountInit(&account_);
   }

  virtual void TearDown() { 
    pthread_mutex_destroy(&account_.lock); 
  }

  lock_t account_;
};
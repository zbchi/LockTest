#include <gtest/gtest.h>

#include <chrono>
#include <random>

#include "list_lock.h"
#include "lock.h"

class LockTest : public ::testing::Test {
 protected:
  virtual void SetUp() { amountInit(&account_); }

  virtual void TearDown() { pthread_mutex_destroy(&account_.lock); }

  lock_t account_;
};

class ListLockTest : public ::testing::Test {
 protected:
  virtual void SetUp() { listInit(&list_); }

  virtual void TearDown() {
    while (list_.data != NULL) {
      LNode* temp = list_.data;
      list_.data = list_.data->next;
      free(temp);
    }

    pthread_cond_destroy(&(list_.cond));
    pthread_mutex_destroy(&(list_.mutex));
  }

  void static RandomSleep() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(1, 100);
    int SleepTime = distribution(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
  }

  list_lock_t list_;
};
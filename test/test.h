#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <random>

extern "C" {
#include "hash_lock.h"
#include "list_lock.h"
#include "lock.h"
}

class LockTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    account_ = std::make_shared<lock_t>();
    amountInit(account_.get());
  }

  virtual void TearDown() { pthread_mutex_destroy(&account_->mutex); }

  std::shared_ptr<lock_t> account_;
};

class ListLockTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    list_ = std::make_shared<list_lock_t>();
    listInit(list_.get());
  }

  virtual void TearDown() {
    pthread_cond_destroy(&list_->cond);
    pthread_mutex_destroy(&list_->mutex);
  }

  void static RandomSleep() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(1, 100);
    int SleepTime = distribution(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
  }

  std::shared_ptr<list_lock_t> list_;
};

class HashLockTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    bucket_ = std::make_shared<hash_lock_t>();
    hashInit(bucket_.get());
  }

  virtual void TearDown() {
    for (int i = 0; i < HASHNUM; i++) {
      pthread_mutex_destroy(&bucket_->table[i].mutex);
    }
  }

  std::shared_ptr<hash_lock_t> bucket_;
};
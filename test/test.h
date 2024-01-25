#include <gtest/gtest.h>

#include <chrono>
#include <random>

#include "hash_lock.h"
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
    pthread_mutex_lock(&list_.mutex);
    while (list_.data != NULL) {
      LNode* temp = list_.data;
      list_.data = list_.data->next;
      free(temp);
    }
    pthread_mutex_unlock(&list_.mutex);

    pthread_cond_destroy(&list_.cond);
    pthread_mutex_destroy(&list_.mutex);
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

class HashLockTest : public ::testing::Test {
 protected:
  virtual void SetUp() { hashInit(&table_); }

  virtual void TearDown() {
    for (int i = 0; i < HASHNUM; i++) {
      pthread_mutex_lock(&table_.table[i].mutex);
      while (table_.table[i].head != NULL) {
        struct HashNode* temp = table_.table[i].head;
        table_.table[i].head = table_.table[i].head->next;
        free(temp);
      }
      pthread_mutex_unlock(&table_.table[i].mutex);
      pthread_mutex_destroy(&table_.table[i].mutex);
    }
  }

  hash_lock_t table_;
};
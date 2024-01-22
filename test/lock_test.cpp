#include <gtest/gtest.h>
#include <thread>
#include <vector>

#include "thread_guard.h"
#include "lock_test.h"

TEST(LockTest,InitTest) {
  lock_test_t metux;
  LockTestInit(&metux);
  ASSERT_EQ(pthread_mutex_trylock(&metux.lock), 0);
  pthread_mutex_unlock(&metux.lock);
}

TEST(LockTest,MutexPutTest_1) {
  std::vector<std::thread> threads;
  lock_test_t metux;
  int balance = 0;
  LockTestInit(&metux);

  std::thread deposit_1{Deposit, metux, std::ref(balance), 20};
  threads.push_back(std::move(deposit_1));
  std::thread deposit_2{Deposit, metux, std::ref(balance), 30};
  threads.push_back(std::move(deposit_2));

  for (auto &t : threads) {
    t.join();
  }

  ASSERT_EQ(balance, 50);
}

TEST(lockTest,MutexPutTest_2) {
  std::vector<std::thread> threads;
  lock_test_t metux;
  int balance = 0;
  const int keys_per_thread = 10000;
  LockTestInit(&metux);

  for (int tid = 1; tid < 4; tid++) {
    std::thread t([&balance, tid, metux] {
      for(uint32_t i = 0; i < keys_per_thread; i++) {
        Deposit(metux, balance, tid);
      }
    });
    threads.push_back(std::move(t));
  }

  for (auto &t : threads) {
    t.join();
  }

  ASSERT_EQ(balance, 6 * keys_per_thread);
}

TEST(lockTest,MutexPutTest_3) {
  std::vector<std::thread> threads;
  lock_test_t metux;
  int balance = 0;
  const int keys_per_thread = 10000;
  LockTestInit(&metux);

  for (int tid = 1; tid < 4; tid++) {
    std::thread t([&balance, tid, metux] {
      for (uint32_t i = 0; i < keys_per_thread; i++) {
        Deposit(metux, balance, tid);
      }
    });
    threads.push_back(std::move(t));
  }

  for (int tid = 1; tid < 4; tid++) {
    std::thread t([&balance, tid, metux] {
      for (uint32_t i = 0; i < keys_per_thread; i++) {
        Withdraw(metux, balance, tid);
      }
    });
    threads.push_back(std::move(t));
  }

  for (auto &t : threads) {
    t.join();
  }

  ASSERT_EQ(balance, 0);
}
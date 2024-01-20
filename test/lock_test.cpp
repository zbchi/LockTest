#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <fstream>

#include "thread_guard.h"
#include "lock_test.h"

TEST(LockTest,InitTest) {
  lock_test_t metux;
  LockTestInit(&metux);
  ASSERT_EQ(pthread_mutex_trylock(&metux.lock), 0);
  pthread_mutex_unlock(&metux.lock);
}

TEST(LockTest, BasicPutTest) {
  int number = 0;

  for (int tid = 0; tid < 5; tid++) {
    std::thread t{AddNumber, number};
    thread_guard g(t);
  }
  ASSERT_EQ(number, 0);
}

TEST(LockTest,MutexPutTest) {
  int number = 0;

  for (int tid = 0; tid < 5; tid++) {
    std::thread t{AddNumber, std::ref(number)};
    thread_guard g(t);
  }
  ASSERT_EQ(number, 5);
}

TEST(LockTest,MutexPutTest1) {
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
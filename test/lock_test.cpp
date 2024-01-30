#include <gtest/gtest.h>
#include <thread>
#include <vector>

#include "test.h"

TEST_F(LockTest, InitTest) {
  ASSERT_EQ(account_.amount, 0);
  ASSERT_EQ(pthread_mutex_trylock(&account_.lock), 0);
  pthread_mutex_unlock(&account_.lock);
}

TEST_F(LockTest, BasicTest) {
  std::thread Income_1{Income, std::ref(account_), 30};
  std::thread Income_2{Income, std::ref(account_), 30};
  std::thread Expend_1{Expend, std::ref(account_), 10};

  Income_1.join();
  Income_2.join();
  Expend_1.join();

  ASSERT_EQ(account_.amount, 50);
}

TEST_F(LockTest, SingleIncrementTest) {
  std::vector<std::thread> threads;
  const int keys_per_thread = 1000;
  auto& account = account_;

  for (int tid = 1; tid < 4; tid++) {
    std::thread t([&account, tid] {
      for (uint32_t i = 0; i < keys_per_thread; i++) {
        Expend(account, tid);
      }
    });
    threads.push_back(std::move(t));
  }

  for (auto &t : threads) {
    t.join();
  }

  ASSERT_EQ(account.amount, 6 * keys_per_thread);
}

TEST_F(LockTest, MutexPutTest) {
  std::vector<std::thread> threads;
  const int keys_per_thread = 1000;
  auto &account = account_;

  for (int tid = 1; tid < 4; tid++) {
    std::thread t([&account, tid] {
      for (uint32_t i = 0; i < keys_per_thread; i++) {
        Income(account, tid);
      }
    });
    threads.push_back(std::move(t));
  }

  for (int tid = 1; tid < 4; tid++) {
    std::thread t([&account, tid] {
      for (uint32_t i = 0; i < keys_per_thread; i++) {
        Expend(account, tid);
      }
    });
    threads.push_back(std::move(t));
  }

  for (auto &t : threads) {
    t.join();
  }

  ASSERT_EQ(account.amount, 0);
}
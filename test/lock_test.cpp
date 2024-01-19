#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include "lock_test.h"

TEST(LockTest,BasicPutTest) {
  std::vector<std::thread> threads;

  int number = 0;

  for (int tid = 0; tid < 100; tid++) {
    std::thread t([&number] { AddNumber(number); });
    threads.push_back(std::move(t));
  }
  ASSERT_EQ(number, 100);
}
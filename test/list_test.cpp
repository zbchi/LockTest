#include <gtest/gtest.h>

#include <chrono>
#include <random>
#include <thread>
#include <vector>

#include "test.h"

TEST_F(ListLockTest, InitTest) {
  ASSERT_EQ(list_.data, NULL) << "List is not initialized...";

  ASSERT_EQ(pthread_mutex_trylock(&list_.mutex), 0)
      << "Lock is not initialized";
  pthread_mutex_unlock(&list_.mutex);

  ASSERT_EQ(pthread_cond_signal(&list_.cond), 0) << "cond is not initialized";
}

TEST_F(ListLockTest, BasicTest) {
  std::vector<std::thread> pro_threads;
  std::vector<std::thread> con_threads;

  for (int tid = 0; tid < 4; tid++) {
    std::thread t{producer, std::ref(list_), tid};
    pro_threads.push_back(std::move(t));
  }

  for (auto& t : pro_threads) {
    t.join();
  }

  ASSERT_EQ(list_.data->value, 0);
  ASSERT_EQ(list_.data->next->next->value, 2);
  ASSERT_EQ(getListSize(list_), 4);

  for (int tid = 0; tid < 4; tid++) {
    std::thread t{consumer, std::ref(list_)};
    con_threads.push_back(std::move(t));
  }

  for (auto& t : pro_threads) {
    t.join();
  }

  ASSERT_EQ(list_.data, NULL) << "Resources not released";
  ASSERT_EQ(getListSize(list_), 0);
}

TEST_F(ListLockTest, MixedConcurrentTest) {
  std::vector<std::thread> threads;
  const int keys_per_thread = 10000;
  auto& list = list_;

  for (int tid = 0; tid < 4; tid++) {
    std::thread t([&list, tid] {
      for (uint32_t i = 0; i < keys_per_thread; i++) {
        producer(list, tid * i);
      }
      for (uint32_t i = 0; i < keys_per_thread; i++) {
        consumer(list);
      }
      for (uint32_t i = 0; i < keys_per_thread; i++) {
        producer(list, tid * i);
      }
    });
    threads.push_back(std::move(t));
  }

  for (auto& t : threads) {
    t.join();
  }

  ASSERT_EQ(getListSize(list), keys_per_thread);

  std::vector<std::thread> con_threads;

  std::thread conthread([&list] {
    for (int i = 0; i < keys_per_thread; i++) {
      consumer(list);
    }
  });

  conthread.join();

  ASSERT_EQ(getListSize(list), 0);
  ASSERT_EQ(list.data, NULL);
}

TEST_F(ListLockTest, LongTimeTest) {
  std::shared_ptr<std::atomic_bool> stop =
      std::make_shared<std::atomic_bool>(false);
  auto& list = list_;

  std::thread pro_thread([&list, stop] {
    while (!stop->load()) {
      producer(list, 1);
      RandomSleep();
    }
  });

  std::thread con_thread([&list, stop] {
    while (!stop->load() || getListSize(list) != 0) {
      consumer(list);
      RandomSleep();
    }
  });

  std::this_thread::sleep_for(std::chrono::seconds(5));

  stop->store(true);

  pro_thread.join();
  con_thread.join();

  ASSERT_EQ(getListSize(list), 0);
  ASSERT_EQ(list.data, NULL);
}
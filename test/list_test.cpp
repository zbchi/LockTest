#include <gtest/gtest.h>
#include <chrono>
#include <random>
#include <thread>
#include <vector>

#include "list_lock.h"
#include "thread_guard.h"

TEST(LockTest, InitTest) {
  list_lock_t list;
  initList(&list);
  // 检查 LinkList 是否被初始化为 NULL
  ASSERT_EQ(list.data, NULL) << "Linked list is not initialized...";

  // 检查互斥锁是否被正确初始化
  ASSERT_EQ(pthread_mutex_trylock(&list.mutex), 0) << "Lock is not initialized";
  pthread_mutex_unlock(&list.mutex);

  // 检查条件变量是否被正确初始化
  ASSERT_EQ(pthread_cond_signal(&list.cond), 0)<< "cond is not initialized";
}

TEST(LockTest,BasicTest) {
    std::vector<std::thread> pro_threads;
    std::vector<std::thread> con_threads;
    list_lock_t list;
    initList(&list);

    for (int tid = 0; tid < 4; tid++) {
      std::thread t{producer, std::ref(list),tid};
      pro_threads.push_back(std::move(t));
    }

    for (auto& t : pro_threads) {
      t.join();
    }

    ASSERT_EQ(list.data->value, 0);
    ASSERT_EQ(list.data->next->next->value, 2);
    ASSERT_EQ(getListSize(list), 4);

    for (int tid = 0; tid < 4; tid++) {
      std::thread t{consumer, std::ref(list)};
      con_threads.push_back(std::move(t));
    }

    for (auto& t : pro_threads) {
      t.join();
    }

    ASSERT_EQ(list.data, NULL) << "Resources not released";
    ASSERT_EQ(getListSize(list), 0);
}

TEST(LockTest, MixedConcurrentTest) {
  list_lock_t list;
  std::vector<std::thread> threads;
  initList(&list);

  const int keys_per_thread = 10000;

  for (int tid = 0; tid < 4; tid ++) {
    std::thread t([&list, tid] { 
      for (uint32_t i = 0; i < keys_per_thread;i++) {
        producer(list, tid * i);
      }
      for (uint32_t i = 0; i < keys_per_thread; i++) {
        consumer(list);
      }
      for (uint32_t i = 0; i < keys_per_thread; i++) {
        producer(list, tid*i);
      }
    });
    threads.push_back(std::move(t));
  }

  for (auto&t : threads) {
    t.join();
  }

  ASSERT_EQ(getListSize(list), keys_per_thread);

  std::vector<std::thread> con_threads;

  std::thread conthread([&list] {
    for (uint32_t i = 0; i < keys_per_thread; i++) {
      consumer(list);
    }
  });

  conthread.join();

  ASSERT_EQ(getListSize(list), 0);
  ASSERT_EQ(list.data, NULL);
}

TEST(LockTest, LongTimeTest) {
  list_lock_t list;
  std::shared_ptr<std::atomic_bool> stop =
      std::make_shared<std::atomic_bool>(false);

  initList(&list);

  std::thread pro_thread([&list, stop] {
    while(!stop->load()) {
      producer(list, 1);
      thread_guard::sleep();
    }
  });

  std::thread con_thread([&list, stop] {
    while(!stop->load() && !getListSize(list)) {
      consumer(list);
      thread_guard::sleep();
    }
  });

  std::this_thread::sleep_for(std::chrono::seconds(2));

  pro_thread.join();
  con_thread.join();

  ASSERT_EQ(getListSize(list), 0);
  ASSERT_EQ(list.data, NULL);
}
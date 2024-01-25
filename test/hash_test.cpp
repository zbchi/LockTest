#include <gtest/gtest.h>

#include <thread>
#include <vector>

#include "test.h"

TEST_F(HashLockTest, InitTest) {
  ASSERT_EQ(table_.table[0].head, NULL) << "hashtable is not initialized...";
  ASSERT_EQ(pthread_mutex_trylock(&table_.table[0].mutex), 0)
      << "Lock is not initialized";
  pthread_mutex_unlock(&table_.table[0].mutex);

  ASSERT_EQ(table_.table[12].head, NULL) << "hashtable is not initialized...";
  ASSERT_EQ(pthread_mutex_trylock(&table_.table[12].mutex), 0)
      << "Lock is not initialized";
  pthread_mutex_unlock(&table_.table[12].mutex);
}

TEST_F(HashLockTest, BasicTest) {
  ASSERT_EQ(getValue(&table_, 1), 0);
  insert(&table_, 1, 11);
  {
    int data = getValue(&table_, 1);
    ASSERT_EQ(data, 11);
  }
  setKey(&table_, 1, 2);
  {
    int data = getValue(&table_, 2);
    ASSERT_EQ(data, 11);
    ASSERT_EQ(table_.table[1].head, NULL) << "Pointer is not NULL";
  }
  ASSERT_EQ(setKey(&table_, 1, 2), 0);
}

TEST_F(HashLockTest, UpdateTest) {
  for (int i = 0; i < HASHNUM; i++) {
    insert(&table_, i, i);
  }
  for (int i = 0; i < HASHNUM; i++) {
    insert(&table_, i, i * (i + 1));
  }
  setKey(&table_, 0, 1);
  ASSERT_EQ(table_.table[0].head, NULL) << "Data not updated";
}

TEST_F(HashLockTest, MixedTest) {
  for (int i = 0; i < HASHNUM * 2; i++) {
    insert(&table_, i, i);
  }
  for (int i = 0; i < HASHNUM * 2; i++) {
    setKey(&table_, i, (HASHNUM + i - 1) % HASHNUM);
  }
  for (int i = 0; i < HASHNUM * 2; i++) {
    ASSERT_EQ(getValue(&table_, i), (HASHNUM + i - 1) % HASHNUM);
  }
}

TEST_F(HashLockTest, MixedConcurrentTest) {
  std::vector<std::thread> threads;

  const int keys_per_thread = 10000;
  auto& table = table_;

  for (int tid = 0; tid < 4; tid++) {
    std::thread t([&table, tid] {
      for (int i = 0; i < keys_per_thread; i++) {
        insert(&table, i + tid, i * 4 + tid);
      }
      for (int i = 0; i < keys_per_thread; i++) {
        setKey(&table, i * 4 + tid, (HASHNUM + i * 4 + tid - 1) % HASHNUM);
      }
    });
    threads.push_back(std::move(t));
  }

  std::vector<std::thread> read_threads;
  std::shared_ptr<std::atomic_bool> stop =
      std::make_shared<std::atomic_bool>(false);

  for (int tid = 0; tid < 4; tid++) {
    std::thread t([&table, tid, stop] {
      int i = 0;
      while (!stop->load()) {
        getValue(&table, i * 4 + tid);
        i = (i + 1) % keys_per_thread;
      }
    });
    read_threads.push_back(std::move(t));
  }

  for(auto &t : threads) {
    t.join();
  }

  stop->store(true);

  for (auto &t : read_threads) {
    t.join();
  }

  for (int i = 0,tid =0; i < keys_per_thread * 4; i++) {
    int data = getValue(&table, i);
    ASSERT_EQ(data, (HASHNUM + i * 4 + tid - 1) % HASHNUM);
    tid = (tid + 1) % 4;
  }
}
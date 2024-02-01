#include <gtest/gtest.h>

#include <thread>
#include <vector>

#include "test.h"

TEST_F(HashLockTest, InitTest) {
  ASSERT_TRUE(bucket_->table[0].head == nullptr)
      << "hashtable is not initialized...";
  ASSERT_EQ(pthread_mutex_trylock(&bucket_->table[0].mutex), 0)
      << "Lock is not initialized";
  pthread_mutex_unlock(&bucket_->table[0].mutex);

  ASSERT_TRUE(bucket_->table[12].head == nullptr)
      << "hashtable is not initialized...";
  ASSERT_EQ(pthread_mutex_trylock(&bucket_->table[12].mutex), 0)
      << "Lock is not initialized";
  pthread_mutex_unlock(&bucket_->table[12].mutex);
}

TEST_F(HashLockTest, BasicTest) {
  ASSERT_EQ(getValue(bucket_.get(), 1), -1);

  insert(bucket_.get(), 1, 11);
  {
    int data = getValue(bucket_.get(), 1);
    ASSERT_EQ(data, 11);
  }
  setKey(bucket_.get(), 1, 2);
  {
    int data = getValue(bucket_.get(), 2);
    ASSERT_EQ(data, 11);
    ASSERT_TRUE(bucket_->table[1].head == nullptr) << "Pointer is not NULL";
  }
  ASSERT_EQ(setKey(bucket_.get(), 1, 2), -1);
}

TEST_F(HashLockTest, UpdateTest) {
  for (int i = 0; i < HASHNUM; i++) {
    insert(bucket_.get(), i, i);
  }
  for (int i = 0; i < HASHNUM; i++) {
    insert(bucket_.get(), i, i * (i + 1));
  }
  setKey(bucket_.get(), 0, 1);
  ASSERT_TRUE(bucket_->table[0].head == nullptr) << "Data not updated";
}

TEST_F(HashLockTest, MixedTest) {
  for (int i = 0; i < HASHNUM * 2; i++) {
    insert(bucket_.get(), i, i);
  }
  for (int i = 0; i < HASHNUM * 2; i++) {
    int value = (i < HASHNUM) ? (HASHNUM - i - 1) % HASHNUM
                              : (HASHNUM * 3 - i - 1) % HASHNUM;
    setKey(bucket_.get(), i, value);
  }

  for (int i = 0; i < HASHNUM; i++) {
    ASSERT_EQ(getValue(bucket_.get(), i), 2 * HASHNUM - i - 1);
  }
}

/* TEST_F(HashLockTest, MixedConcurrentTest) {
  std::vector<std::thread> threads;

  const int keys_per_thread = 10000;
  auto &bucket = bucket_;

  for (int tid = 0; tid < 4; tid++) {
    std::thread t([&bucket, tid] {
      for (int i = 0; i < keys_per_thread; i++) {
        insert(bucket.get(), i + tid, i * 4 + tid);
      }
      for (int i = 0; i < keys_per_thread; i++) {
        setKey(bucket.get(), i * 4 + tid, (HASHNUM + i * 4 + tid - 1) %
HASHNUM);
      }
    });
    threads.push_back(std::move(t));
  }

  std::vector<std::thread> read_threads;
  std::shared_ptr<std::atomic_bool> stop =
      std::make_shared<std::atomic_bool>(false);

  for (int tid = 0; tid < 4; tid++) {
    std::thread t([&bucket, tid, stop] {
      int i = 0;
      while (!stop->load()) {
        getValue(bucket.get(), i * 4 + tid);
        i = (i + 1) % keys_per_thread;
      }
    });
    read_threads.push_back(std::move(t));
  }

  for (auto &t : threads) {
    t.join();
  }

  stop->store(true);

  for (auto &t : read_threads) {
    t.join();
  }

  for (int i = 0, tid = 0; i < keys_per_thread * 4; i++) {
    int data = getValue(bucket.get(), i);
    ASSERT_EQ(data, (HASHNUM + i * 4 + tid - 1) % HASHNUM);
    tid = (tid + 1) % 4;
  }
} */
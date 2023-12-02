#include "Lock.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <gtest/gtest.h>
#include <stdlib.h>
extern pthread_mutex_t mutex;
extern int number;

TEST(LockTest1A,mutex1) {
  pthread_mutex_init(&mutex, NULL);
  pthread_t p1, p2;
  pthread_create(&p1, NULL, funcA_num, NULL);
  pthread_create(&p2, NULL, funcB_num, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  pthread_mutex_destroy(&mutex);
  EXPECT_EQ(number, 200);
}
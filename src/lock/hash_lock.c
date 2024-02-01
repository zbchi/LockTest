#include "hash_lock.h"

#include <stdio.h>
#include <stdlib.h>

void hashInit(hash_lock_t* bucket) {
  for (int i = 0; i < HASHNUM; ++i) {
    pthread_mutex_init(&bucket->table[i].mutex, NULL);
    bucket->table[i].head = NULL;
  }
}

int getValue(hash_lock_t* bucket, int key) {
  int index = HASH(key);
  pthread_mutex_lock(&bucket->table[index].mutex);
  Hnode* current = bucket->table[index].head;
  while (current != NULL) {
    if (current->key == key) {
      int value = current->value;
      pthread_mutex_unlock(&bucket->table[index].mutex);
      return value;
    }
    current = current->next;
  }
  pthread_mutex_unlock(&bucket->table[index].mutex);
  return -1;
}

void insert(hash_lock_t* bucket, int key, int value) {
  int index = HASH(key);

  // Find if key already exists
  pthread_mutex_lock(&bucket->table[index].mutex);
  Hnode* current = bucket->table[index].head;
  while (current != NULL) {
    if (current->key == key) {
      current->value = value;
      pthread_mutex_unlock(&bucket->table[index].mutex);
      return;
    }
    current = current->next;
  }

  // key does not exist
  pthread_mutex_unlock(&bucket->table[index].mutex);
  Hnode* new_node = (Hnode*)malloc(sizeof(Hnode));
  new_node->key = key;
  new_node->value = value;

  pthread_mutex_lock(&bucket->table[index].mutex);
  new_node->next = bucket->table[index].head;
  bucket->table[index].head = new_node;
  pthread_mutex_unlock(&bucket->table[index].mutex);
}

int setKey(hash_lock_t* bucket, int key, int new_key) {
  int old_index = HASH(key);
  int old_value;
  pthread_mutex_lock(&bucket->table[old_index].mutex);

  Hnode* old_node = bucket->table[old_index].head;
  Hnode* pre_node = NULL;

  while (old_node != NULL) {
    if (old_node->key == key) {
      if (pre_node == NULL) {
        bucket->table[old_index].head = old_node->next;
      } else {
        pre_node->next = old_node->next;
      }
      old_value = old_node->value;
      break;
    }
    pre_node = old_node;
    old_node = old_node->next;
  }

  pthread_mutex_unlock(&bucket->table[old_index].mutex);
  if (old_node == NULL) {
    return -1;  // not find
  }

  free(old_node);
  insert(bucket, new_key, old_value);

  return 0;
}
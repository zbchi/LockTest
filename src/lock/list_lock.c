#include "list_lock.h"

#include <stdio.h>
#include <stdlib.h>

void listInit(list_lock_t* list) {
  list->head = NULL;
  pthread_mutex_init(&list->mutex, NULL);
  pthread_cond_init(&list->cond, NULL);
}

void producer(list_lock_t* list, DataType value) {
  LNode* newNode = (LNode*)malloc(sizeof(LNode));
  if (newNode == NULL) {
    perror("Error allocating memory for new node");
    return;
  }
  newNode->value = value;
  newNode->next = NULL;

  pthread_mutex_lock(&list->mutex);

  newNode->next = list->head;
  list->head = newNode;

  pthread_mutex_unlock(&list->mutex);
  pthread_cond_broadcast(&list->cond);
}
void consumer(list_lock_t* list) {
  pthread_mutex_lock(&list->mutex);

  while (list->head == NULL) {
    pthread_cond_wait(&list->cond, &list->mutex);
  }

  LNode* current = list->head;
  list->head = current->next;
  free(current);

  pthread_mutex_unlock(&list->mutex);
}


int getListSize(list_lock_t* list) {
  pthread_mutex_lock(&list->mutex);

  int number = 0;
  LNode* current = list->head;
  while (current != NULL) {
    current = current->next;
    number++;
  }

  pthread_mutex_unlock(&list->mutex);
  return number;
}
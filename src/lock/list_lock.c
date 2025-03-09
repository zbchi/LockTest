#include "list_lock.h"

#include <stdio.h>
#include <stdlib.h>

void listInit(list_lock_t* list) {
  pthread_mutex_lock(&(list->mutex));
  list->head=NULL;
  pthread_mutex_unlock(&(list->mutex));
}

void producer(list_lock_t* list, DataType value) {
  pthread_mutex_lock(&(list-> mutex));
  LinkList NewNode=(LinkList)malloc(sizeof(LNode));
  NewNode->value=value;
  NewNode->next=list->head;
  list->head=NewNode;
  pthread_mutex_unlock(&(list->mutex));
  pthread_cond_signal(&(list->cond));
}

void consumer(list_lock_t* list) {
  LinkList consumption;
  pthread_mutex_lock(&(list->mutex));
  while(list->head==NULL)
    pthread_cond_wait(&(list->cond),&(list->mutex));
  consumption=list->head;
  list->head=list->head->next;
  free(consumption);
  pthread_mutex_unlock(&(list->mutex));
  
}

int getListSize(list_lock_t* list) {
  int size=0;
  LinkList pCurrent=list->head;
  while(pCurrent!=NULL)
  {
    pCurrent=pCurrent->next;
    size++;
  }
  return size;
}
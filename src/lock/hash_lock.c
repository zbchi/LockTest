#include "hash_lock.h"

#include <stdio.h>
#include <stdlib.h>

void hashInit(hash_lock_t* bucket) 
{
  for(int i=0;i<HASHNUM;i++)
  {
    pthread_mutex_lock(&(bucket->table[i].mutex));
    bucket->table[i].head=NULL;
    pthread_mutex_unlock(&(bucket->table[i].mutex));
  }
}

int getValue(hash_lock_t* bucket, int key) {
  int i=HASH(key);
  pthread_mutex_lock(&(bucket->table[i].mutex));
  Hlist pCurrent=bucket->table[i].head;
  while(pCurrent!=NULL&&key!=pCurrent->key)
  {
    pCurrent=pCurrent->next;
  }
  pthread_mutex_unlock(&(bucket->table[i].mutex));
  if(pCurrent==NULL)return -1;
  else return pCurrent->value;
}

void insert(hash_lock_t* bucket, int key, int value) 
{
  int i=HASH(key);
  pthread_mutex_lock(&(bucket->table[i].mutex));
  Hlist pCurrent=bucket->table[i].head;
  while(pCurrent!=NULL&&key!=pCurrent->key)
  {
    pCurrent=pCurrent->next;
  }
  if(pCurrent!=NULL)
  pCurrent->value=value;
  else
  {//没找到直接前插

    Hlist NewNode=(Hlist)malloc(sizeof(Hnode));
    NewNode->key=key;
    NewNode->value=value;
    
    Hlist tmp=bucket->table[i].head;
    bucket->table[i].head=NewNode;
    NewNode->next=tmp;
  }
  pthread_mutex_unlock(&(bucket->table[i].mutex));
}

int setKey(hash_lock_t* bucket, int key, int new_key) 
{
  int i=HASH(key);
  pthread_mutex_lock(&(bucket->table[i].mutex));
  Hlist pCurrent=bucket->table[i].head;
  Hlist pPrev=NULL;
  while(pCurrent!=NULL&&pCurrent->key!=key)
  {
    pPrev=pCurrent;
    pCurrent=pCurrent->next;
  }
  if(pCurrent==NULL)
  { 
    pthread_mutex_unlock(&(bucket->table[i].mutex));
    return -1;
  }
  else
  {
      Hlist move_node=pCurrent;
      if(pPrev==NULL)
      {//头删
        bucket->table[i].head=pCurrent->next;
      }
      else
      {
        pPrev->next=pCurrent->next;
      }
      pthread_mutex_unlock(&(bucket->table[i].mutex));

      int new_i=HASH(new_key);

      pthread_mutex_lock(&(bucket->table[new_i].mutex));
      Hlist tmp=bucket->table[new_i].head;

      bucket->table[new_i].head=move_node;
      move_node->next=tmp;
      //键值换成new
      move_node->key=new_key;
      pthread_mutex_unlock(&(bucket->table[new_i].mutex));
      return 0;
  }
}


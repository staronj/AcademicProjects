/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "mutex_and_signal.h"
#include "safe_memory_operations.h"
#include "err.h"
#include <pthread.h>
#include <assert.h>
#include <malloc.h>

struct MutexAndSignal {
  pthread_mutex_t lock;
  pthread_cond_t signal;
};

MutexAndSignal_pointer MutexAndSignal_create() {
  MutexAndSignal_pointer this = safe_raw_allocate(1, sizeof(struct MutexAndSignal));
  if (pthread_mutex_init(&this->lock, 0) != 0)
    syserr ("MutexAndSignal_create: gMutex init failed");
  if (pthread_cond_init(&this->signal, 0) != 0)
    syserr ("MutexAndSignal_create: signal init failed");
  return this;
}

void MutexAndSignal_lock(MutexAndSignal_pointer this) {
  assert(this != NULL);
  if (pthread_mutex_lock(&this->lock) != 0)
    syserr ("MutexAndSignal_lock lock failed");
}

void MutexAndSignal_unlock(MutexAndSignal_pointer this) {
  assert(this != NULL);
  if (pthread_mutex_unlock(&this->lock) != 0)
    syserr ("MutexAndSignal_unlock unlock failed");
}

void MutexAndSignal_wait(MutexAndSignal_pointer this) {
  assert(this != NULL);
  if (pthread_cond_wait(&this->signal, &this->lock) != 0)
    syserr("MutexAndSignal_wait cond wait failed");
}

void MutexAndSignal_signal(MutexAndSignal_pointer this) {
  if (pthread_cond_signal(&this->signal) != 0)
    syserr ("MutexAndSignal_signal cond signal failed");
}

void MutexAndSignal_free(MutexAndSignal_pointer this) {
  assert(this != NULL);
  if (pthread_cond_destroy (&this->signal) != 0)
    syserr ("MutexAndSignal_free cond destroy failed");
  if (pthread_mutex_destroy (&this->lock) != 0)
    syserr ("MutexAndSignal_free gMutex destroy failed");

  free(this);
}

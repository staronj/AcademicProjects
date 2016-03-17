/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "../system_headers.h"
#include "activity_suspender.h"
#include "../safe_memory_operations.h"
#include "../mutex_and_signal.h"

struct ActivitySuspender {
  bool suspended;
  MutexAndSignal_pointer mutex_signal;
};

ActivitySuspender_pointer ActivitySuspender_create() {
  ActivitySuspender_pointer this = safe_raw_allocate(1, sizeof(struct ActivitySuspender));
  this->suspended = false;
  this->mutex_signal = MutexAndSignal_create();
  return this;
}

void ActivitySuspender_pass(ActivitySuspender_pointer this) {
  assert(this != NULL);
  MutexAndSignal_lock(this->mutex_signal);
  while (this->suspended) {
    MutexAndSignal_wait(this->mutex_signal);
  }
  MutexAndSignal_signal(this->mutex_signal);
  MutexAndSignal_unlock(this->mutex_signal);
}

void ActivitySuspender_suspend(ActivitySuspender_pointer this) {
  assert(this != NULL);
  MutexAndSignal_lock(this->mutex_signal);
  this->suspended = true;
  MutexAndSignal_unlock(this->mutex_signal);
}

void ActivitySuspender_resume(ActivitySuspender_pointer this) {
  assert(this != NULL);
  MutexAndSignal_lock(this->mutex_signal);
  this->suspended = false;
  MutexAndSignal_signal(this->mutex_signal);
  MutexAndSignal_unlock(this->mutex_signal);
}

void ActivitySuspender_free(ActivitySuspender_pointer this) {
  assert(this != NULL);
  MutexAndSignal_free(this->mutex_signal);
  free(this);
}


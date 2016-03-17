/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "thread.h"
#include "safe_memory_operations.h"
#include "err.h"

struct Thread {
  pthread_t thread_id;
  bool joinable;
};

Thread_pointer Thread_create(Thread_function_type function, void* args, bool joinable) {
  pthread_attr_t attr;

  if (pthread_attr_init(&attr) != 0)
    syserr("Thread_create::pthread_attr_init");

  pthread_attr_setdetachstate(&attr, joinable ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED );

  pthread_t	thread_id;
  if (pthread_create(&thread_id, &attr, function, args) != 0)
    syserr("Thread_create::pthread_create");

  Thread_pointer result = safe_raw_allocate(1, sizeof(struct Thread));
  result->joinable = joinable;
  result->thread_id = thread_id;

  if (pthread_attr_destroy(&attr) != 0)
    syserr("Thread_create::pthread_attr_destroy");

  return result;
}

void Thread_joinAndFree(Thread_pointer this, void** result) {
  assert(this != NULL);
  assert(this->joinable);

  if (pthread_join(this->thread_id, result) != 0)
    syserr("Thread_joinAndFree::pthread_join");

  Thread_free(this);
}

void Thread_free(Thread_pointer this) {
  free(this);
}

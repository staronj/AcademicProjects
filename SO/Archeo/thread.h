/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_THREAD_H
#define PROJECT_THREAD_H

#include "system_headers.h"

struct Thread;
typedef struct Thread* Thread_pointer;
typedef void* (*Thread_function_type)(void*);

/**
 * Create and run thread.
 */
Thread_pointer Thread_create(Thread_function_type function, void* args, bool joinable);

/**
 * Join thread. Result - pointer for memory where result will be copied. Ignored if NULL.
 */
void Thread_joinAndFree(Thread_pointer this, void** result);

void Thread_free(Thread_pointer this);


#endif //PROJECT_THREAD_H

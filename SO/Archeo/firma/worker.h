/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_WORKER_H
#define PROJECT_WORKER_H

#include "../system_headers.h"
#include "../mutex_and_signal.h"
#include "../thread.h"
#include "../vector.h"

struct FactorizeEntry {
  int prime;
  int power;
};

void Work();
void* Worker(void *);
void SendWorkReport(int index, Vector_pointer artifacts);

Vector_pointer Factorize(int n, int A);

#endif //PROJECT_WORKER_H

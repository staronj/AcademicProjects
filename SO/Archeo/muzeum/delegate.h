/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_DELEGAT_H
#define PROJECT_DELEGAT_H

struct PermissionInfo {
  int Fid, L, Fk, G;
};

void* DelegateThread(void*);

#endif //PROJECT_DELEGAT_H

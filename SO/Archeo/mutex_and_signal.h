/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_MUTEX_AND_SIGNAL_H
#define PROJECT_MUTEX_AND_SIGNAL_H

/**
 * Para mutex i zmienna warunkowa.
 * Klasa pomocnicza przy definiowaniu monitorów.
 */

struct MutexAndSignal;
typedef struct MutexAndSignal* MutexAndSignal_pointer;

MutexAndSignal_pointer MutexAndSignal_create();
void MutexAndSignal_lock(MutexAndSignal_pointer this);
void MutexAndSignal_unlock(MutexAndSignal_pointer this);
void MutexAndSignal_wait(MutexAndSignal_pointer this);
void MutexAndSignal_signal(MutexAndSignal_pointer this);
void MutexAndSignal_free(MutexAndSignal_pointer this);

#endif //PROJECT_MUTEX_AND_SIGNAL_H

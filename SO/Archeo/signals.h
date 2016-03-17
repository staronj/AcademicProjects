/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_SIGNALS_H
#define PROJECT_SIGNALS_H
#include <signal.h>
#include <sys/types.h>

typedef void(*SignalHandler_type)(int);

/**
 * Set signal handler.
 */
void Signal_setHandler(int signal, SignalHandler_type handler);

/**
 * Ignore signal (set handler to SIG_IGN)
 */
void Signal_ignoreSignal(int signal);

/**
 * Send signal to process with pid.
 */
void Signal_sendSignalToProcess(pid_t pid, int signal);

#endif //PROJECT_SIGNALS_H

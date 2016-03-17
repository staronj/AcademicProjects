/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include <signal.h>
#include "signals.h"
#include "err.h"

void Signal_setHandler(int signal, SignalHandler_type handler) {
  struct sigaction setup_action;
  sigset_t block_mask;

  sigemptyset (&block_mask);
  setup_action.sa_handler = handler;
  setup_action.sa_mask = block_mask;
  setup_action.sa_flags = SA_RESTART;

  if (sigaction (signal, &setup_action, 0) == -1)
    syserr("Signal_setHandler - sigaction");
}

void Signal_ignoreSignal(int signal) {
  Signal_setHandler(signal, SIG_IGN);
}

void Signal_sendSignalToProcess(pid_t pid, int signal) {
  if (kill(pid, signal) == -1)
    syserr("Signal_sendSignalToProcess - kill");
}

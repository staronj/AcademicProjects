/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "worker.h"
#include "firma.h"

struct WorkerInfo {
  int index;
  Thread_pointer thread;
};

void Work() {
  struct WorkerInfo* workersInfo = safe_raw_allocate((size_t)gFk, sizeof(struct WorkerInfo));
  for (size_t i = 0 ; i < gFk ; i++) {
    workersInfo[i].index = gPermissionInfo.L + i;
    workersInfo[i].thread = Thread_create(Worker, &(workersInfo[i]), true);
  }

  for (size_t i = 0 ; i < gFk ; i++) {
    Thread_joinAndFree(workersInfo[i].thread, NULL);
  }

  free(workersInfo);
}

void* Worker(void * pointer) {
  struct WorkerInfo* info = (struct WorkerInfo*)pointer;

  while (true) {
    if (gSigintReceived) {
      send_int(gMessageQueue, DELEGATE_POSTBOX(gFid), DELEGATE_I_WANT_TO_END, info->index);
      break;
    }
    ActivitySuspender_pass(gActivitySuspender);

    send_int(gMessageQueue, DELEGATE_POSTBOX(gFid), DELEGATE_I_WANT_TO_DIG, info->index);
    InputStream_pointer message = receive(gMessageQueue, FIELD_POSTBOX(info->index));
    int type = InputStream_readChar(message);

    if (type == WORKER_FIELD_WORN) {
      InputStream_free(message);
      DEBUG(puts("Worker - field worn, koncze."));
      break;
    }
    else if (type != WORKER_FIELD_SYMBOL) {
      fatal("firma - Work - unknown message type, %d", type);
    }

    int symbol = InputStream_readRawInt(message);
    InputStream_free(message);

    DEBUG(printf("Worker - otrzymalem symbol: %d\n", symbol));
    ActivitySuspender_pass(gActivitySuspender);

    Vector_pointer artifacts = Factorize(symbol, gA);

    ActivitySuspender_pass(gActivitySuspender);
    DEBUG(sleep(2));

    SendWorkReport(info->index, artifacts);


    // czekamy na potwierdzenie, przecież i tak
    // zaakceptują bo nikt nie czyta raportów
    message = receive(gMessageQueue, FIELD_POSTBOX(info->index));
    InputStream_free(message);

    ActivitySuspender_pass(gActivitySuspender);

    MutexAndSignal_lock(gMutex);

    for (size_t i = 0 ; i < Vector_size(artifacts) ; i++) {
      struct FactorizeEntry entry = VECTOR_AT(artifacts, i, struct FactorizeEntry);
      gOwnedArtifacts[entry.prime] += entry.power;

      DEBUG(printf("Worker - wydobylem (%d, %d)\n", entry.prime, entry.power));
    }
    MutexAndSignal_unlock(gMutex);
    Vector_free(artifacts);
  }
  return NULL;
}

Vector_pointer Factorize(int n, int A) {
  Vector_pointer result = Vector_new(sizeof(struct FactorizeEntry), VECTOR_DEFAULT);
  for (int i = 2 ; i <= A && n > 1 ; i++) {
    struct FactorizeEntry entry;
    entry.prime = i;
    entry.power = 0;

    while (n % i == 0) {
      entry.power++;
      n /= i;
    }

    if (entry.power > 0)
      Vector_pushBack(result, &entry);
  }
  return result;
}

void SendWorkReport(int index, Vector_pointer artifacts) {
  StringBuilder_pointer message = StringBuilder_createEmpty();
  StringBuilder_appendChar(message, DELEGATE_WORK_REPORT);
  StringBuilder_appendRawInt(message, index);
  StringBuilder_appendRawInt(message, Vector_size(artifacts));

  /*
   * wiemy, że n nie może być iloczynem zbyt wielu różnych liczb pierwszych
   * tą liczbę da się chyba ograniczyć przez 7, co daje grubo bezpieczne
   * ograniczenie na wielkość wiadomości
   */
  for (size_t i = 0 ; i < Vector_size(artifacts) ; i++) {
    struct FactorizeEntry entry = VECTOR_AT(artifacts, i, struct FactorizeEntry);
    StringBuilder_appendRawInt(message, entry.prime);
    StringBuilder_appendRawInt(message, entry.power);
  }

  send(gMessageQueue, DELEGATE_POSTBOX(gFid), message);
  StringBuilder_free(message);
}


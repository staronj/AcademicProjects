/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "delegate.h"
#include "muzeum.h"

void* DelegateThread(void* args_pointer) {
  struct PermissionInfo* args = (struct PermissionInfo*)args_pointer;
  assert(0 < args->G && args->G <= gGlebokosc);
  MessageBuffer_pointer message_buffer = MessageBuffer_create(gMessageQueue, DELEGATE_POSTBOX(args->Fid));

  int wornFields = 0;
  while (wornFields < args->Fk) {
    InputStream_pointer message = MessageBuffer_get(message_buffer);
    int type = InputStream_readChar(message);

    switch (type) {
      case DELEGATE_I_WANT_TO_DIG:
      {
        int index = InputStream_readRawInt(message);
        DEBUG(printf("Delegate - I want to dig, index=%d\n", index));
        if (gFirstNotExcavated[index - 1] < args->G) {
          int symbol = gTeren[index - 1][gFirstNotExcavated[index - 1]];
          DEBUG(printf("Delegate - wysylam symbol %d\n", symbol));
          send_int(gMessageQueue, FIELD_POSTBOX(index), WORKER_FIELD_SYMBOL, symbol);
        }
        else {
          send_int(gMessageQueue, FIELD_POSTBOX(index), WORKER_FIELD_WORN, index);
          wornFields++;
        }
        break;
      }
      case DELEGATE_I_WANT_TO_END:
      {
        int index = InputStream_readRawInt(message);
        DEBUG(printf("Delegate - I want to end, index=%d\n", index));
        wornFields++;
        break;
      }
      case DELEGATE_WORK_REPORT:
      {
        DEBUG(puts("Delegate - DELEGATE_WORK_REPORT"));
        /*
         * Jak to bywa w biurokracji - dokumentacja musi być, ale
         * i tak nikt jej nie czyta (oprócz kontroli podatkowej i księgowych)
         *
         * W ramach tej zasady nie sprawdzamy, czy worker faktycznie wydobył
         * to co zadeklarował. Nie będziemy przecież przekopywać ziemii...
         * (tzn faktoryzować)
         */
        int index = InputStream_readRawInt(message);
        send_simple(gMessageQueue, FIELD_POSTBOX(index), WORKER_RAPORT_OK);
        DEBUG(puts("Delegate - wyslalem akceptacje raportu."));
        gSzacunek[index - 1][gFirstNotExcavated[index - 1]] = 0;
        gFirstNotExcavated[index - 1]++;
        break;
      }
      default:
        fatal("delegate - unknown message type");
    }
    InputStream_free(message);
  }
  free(args);
  MessageBuffer_killYourselfAndFree(message_buffer);
  return NULL;
}
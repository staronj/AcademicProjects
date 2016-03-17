/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "firma.h"

int main(int argc, char * argv[]) {
  //DEBUG(puts("Firma - uruchomiono."));
  InitializeFirm(argc, argv);
  //DEBUG(puts("Firma - zainicjowano."));

  int saldo = GetSaldo(gMessageQueue, gFid, FIRM_POSTBOX(gFid));
  DEBUG(printf("Tu firma %d o liczbie pracownikow %d i saldzie %d\n", gFid, gFk, saldo));

  while (saldo >= gS) {
    if (gSigintReceived)
      break;
    ActivitySuspender_pass(gActivitySuspender);

    //DEBUG(puts("Firma - prosze o pozwolenie."));
    AskForWorkPermission(saldo);

    ActivitySuspender_pass(gActivitySuspender);

    if (gPermissionInfo.granted)
      Work();

    if (gSigintReceived)
      break;
    ActivitySuspender_pass(gActivitySuspender);

    SellArtifacts();

    if (gSigintReceived)
      break;
    ActivitySuspender_pass(gActivitySuspender);

    //DEBUG(puts("Firma - uaktualniam saldo."));
    saldo = GetSaldo(gMessageQueue, gFid, FIRM_POSTBOX(gFid));
  }

  DEBUG(printf("Firma %d konczy dzialnosc.\n", gFid));
  CleanupFirm();
  return 0;
}


void InitializeFirm(int argc, char ** argv) {
  if (argc != 5)
    fatal("firma - wrong number of arguments");

  gMessageQueue = MessageQueue_open(MESSAGE_QUEUE_KEY);
  gS = safe_cstring_to_long(argv[1]);
  gA = safe_cstring_to_long(argv[2]);
  gFid = safe_cstring_to_long(argv[3]);
  gFk = safe_cstring_to_long_min_max(argv[4], 0, LONG_MAX);

  Signal_setHandler(SIGINT, SigintHandlerFirma);
  gOwnedArtifacts = (int*)safe_raw_allocate((size_t)gA + 1, sizeof(int));
  gMutex = MutexAndSignal_create();

  gActivitySuspender = ActivitySuspender_create();
  Signal_setHandler(SIGUSR1, SigusrHandlerFirma);
  Signal_setHandler(SIGUSR2, SigusrHandlerFirma);
}

void CleanupFirm() {
  { // Wysyłamy do muzeum raport firmy
    InputStream_pointer raport = GenerateFirmReport();
    while (InputStream_hasNext(raport)) {
      size_t size_of_message = MESSAGE_MAX_SIZE - sizeof(int) - sizeof(char);
      String_pointer raport_part = InputStream_readAtMost(raport, size_of_message);
      StringBuilder_pointer message = StringBuilder_createEmpty();
      StringBuilder_appendChar(message, MUZEUM_RAPORT_PART);
      StringBuilder_appendRawInt(message, gFid);
      StringBuilder_appendString(message, raport_part);
      String_free(raport_part);
      send(gMessageQueue, MUZEUM_POSTBOX, message);
      StringBuilder_free(message);
    }
    InputStream_free(raport);

    send_int(gMessageQueue, MUZEUM_POSTBOX, MUZEUM_RAPORT_END, gFid);
  }

  send_int(gMessageQueue, BANK_POSTBOX, FIRM_READY_TO_DEATH, gFid);

  MessageQueue_free(gMessageQueue);
  gMessageQueue = NULL;

  free(gOwnedArtifacts);
  gOwnedArtifacts = NULL;

  MutexAndSignal_free(gMutex);
  ActivitySuspender_free(gActivitySuspender);

  gFid = gFk = 0;
}

void AskForWorkPermission(int Z) {
  {
    StringBuilder_pointer message = StringBuilder_createEmpty();
    StringBuilder_appendChar(message, FIRM_REQUEST_FOR_PERMISSION);
    StringBuilder_appendRawInt(message, gFid);
    StringBuilder_appendRawInt(message, gFk);
    StringBuilder_appendRawInt(message, Z);
    send(gMessageQueue, MUZEUM_POSTBOX, message);
    StringBuilder_free(message);
  }

  InputStream_pointer message = receive(gMessageQueue, FIRM_POSTBOX(gFid));
  assert(InputStream_hasNext(message));
  int type = InputStream_readChar(message);
  if (type == MUZEUM_REQUEST_ACCEPTED) {
    gPermissionInfo.granted = true;
    gPermissionInfo.L = InputStream_readRawInt(message);
    gPermissionInfo.G = InputStream_readRawInt(message);
  }
  else if (type == MUZEUM_REQUEST_REJESTED){
    gPermissionInfo.granted = false;
  }
  else {
    assert(false);
  }
  InputStream_free(message);
}

void SellArtifacts() {
  for (size_t i = 1 ; i <= gA ; i++) {
    while (gOwnedArtifacts[i] >= i) {
      {
        StringBuilder_pointer message = StringBuilder_createEmpty();
        StringBuilder_appendChar(message, MUZEUM_SELL_ARTIFACTS);
        StringBuilder_appendRawInt(message, gFid);
        StringBuilder_appendRawInt(message, i);
        StringBuilder_appendRawInt(message, i);
        send(gMessageQueue, MUZEUM_POSTBOX, message);
        StringBuilder_free(message);
      }

      InputStream_pointer response = receive(gMessageQueue, FIRM_POSTBOX(gFid));
      int type = InputStream_readChar(response);
      InputStream_free(response);
      if (type == FIRM_SELL_NOT_OK) {
        break;
      }
      else if (type == FIRM_SELL_OK) {
        gOwnedArtifacts[i] -= i;
      }
      else {
        fatal("firma - reseived unknown message, %d", type);
      }
    }
  }
}

InputStream_pointer GenerateFirmReport() {
  StringBuilder_pointer buffer = StringBuilder_createEmpty();
  StringBuilder_appendFormattedInt(buffer, gFid);
  StringBuilder_appendChar(buffer, ' ');
  int saldo = GetSaldo(gMessageQueue, gFid, FIRM_POSTBOX(gFid));
  StringBuilder_appendFormattedInt(buffer, saldo);
  StringBuilder_appendChar(buffer, '\n');

  for (size_t i = 0 ; i <= gA ; i++) {
    if (gOwnedArtifacts[i] > 0) {
      StringBuilder_appendFormattedInt(buffer, i);
      StringBuilder_appendChar(buffer, ' ');
      StringBuilder_appendFormattedInt(buffer, gOwnedArtifacts[i]);
      StringBuilder_appendChar(buffer, '\n');
    }
  }

  String_pointer string = StringBuilder_toString(buffer);
  StringBuilder_free(buffer);
  InputStream_pointer result = InputStream_fromString(string);
  String_free(string);
  return result;
}

void SigintHandlerFirma(int n) {
  Signal_ignoreSignal(SIGINT);
  gSigintReceived = true;
  DEBUG(puts("Firma - tu sigint handler."));
}

void SigusrHandlerFirma(int n) {
  switch (n) {
    case SIGUSR1:
      DEBUG(puts("Firma - otrzymalam sigusr1"));
      ActivitySuspender_suspend(gActivitySuspender);
      break;
    case SIGUSR2:
      DEBUG(puts("Firma - otrzymalam sigusr2"));
      ActivitySuspender_resume(gActivitySuspender);
      break;
    default:
      ;
  }
}

int gS = 0;
int gA = 0;
int gFid = 0;
int gFsaldo = 0;
int gFk = 0;
MessageQueue_pointer gMessageQueue = NULL;
int* gOwnedArtifacts = NULL;
struct PermissionInfo gPermissionInfo;
MutexAndSignal_pointer gMutex = NULL;
bool gSigintReceived = false;
ActivitySuspender_pointer gActivitySuspender = NULL;
/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "bank.h"
#include "../utility.h"
#include "../thread.h"

int main(int argc, char * argv[]) {
  InitializeBank(argc, argv);
  DEBUG(puts("Bank - zainicjowano."));
  WaitForMuzeumStart();
  DEBUG(puts("Bank - rozpoczynam dzialanie."));
  StartAllFirms();
  DEBUG(puts("Bank - uruchomilem firmy."));

  while (gDeadFirms < gF) {
    //DEBUG(puts("Bank - czekam na wiadomosc."));
    InputStream_pointer message = MessageBuffer_get(gMessageBuffer);
    //DEBUG(puts("Bank - odebralem wiadomosc."));
    int type = InputStream_readChar(message);

    switch (type) {
      case BANK_SHUTDOWN:
      {
        //DEBUG(puts("Bank - odebralem BANK_SHUTDOWN."));
        for (int Fid = 1 ; Fid <= gF ; Fid++)
          SendShutDownToFirm(Fid);
        break;
      }
      case BANK_GET_SALDO:
      {
        //DEBUG(puts("Bank - odebralem BANK_GET_SALDO."));
        int who = InputStream_readRawInt(message);
        int postbox = InputStream_readRawInt(message);
        GetMySaldo(who, postbox);
        break;
      }
      case BANK_SEND_MONEY:
      {
        //DEBUG(puts("Bank - odebralem BANK_SEND_MONEY."));
        int from = InputStream_readRawInt(message);
        int to = InputStream_readRawInt(message);
        int how_many = InputStream_readRawInt(message);
        int postbox = InputStream_readRawInt(message);
        SendMoney(from, to, how_many, postbox);
        break;
      }
      case FIRM_READY_TO_DEATH:
      {
        int Fid = InputStream_readRawInt(message);
        //DEBUG(printf("Bank - Firma %d zglosila swoja smierc.\n", Fid));
        assert(0 < Fid && Fid <= gF);
        struct Firma* firma = &gFirmy[Fid - 1];
        if (waitpid(firma->pid, 0, WUNTRACED) != firma->pid)
          syserr("Error in waitpid\n");

        firma->pid = 0;
        gDeadFirms++;
        break;
      }
      default:
        fatal("bank - reseived unknown message");
    }
    InputStream_free(message);
  }

  CleanupBank();
  return 0;
}

void InitializeBank(int argc, char ** argv) {
  if (argc != 4)
    fatal("bank - wrong number of arguments");
  gF = safe_cstring_to_long_min_max(argv[1], 1, LONG_MAX);
  gS = safe_cstring_to_long_min_max(argv[2], 1, LONG_MAX);
  gA = safe_cstring_to_long_min_max(argv[3], 1, LONG_MAX);
  gFirmy = safe_raw_allocate((size_t)gF, sizeof(struct Firma));
  gMessageQueue = MessageQueue_openOrCreate(MESSAGE_QUEUE_KEY);
  gMessageBuffer = MessageBuffer_create(gMessageQueue, BANK_POSTBOX);

  for (size_t i = 0 ; i < gF ; i++) {
    int Fid, Fsaldo, Fk;
    scanf("%d %d %d", &Fid, &Fsaldo, &Fk);
    assert(0 < Fid && Fid <= gF);
    gFirmy[Fid - 1].Fid = Fid;
    gFirmy[Fid - 1].Fsaldo = Fsaldo;
    gFirmy[Fid - 1].Fk = Fk;
  }
}

void WaitForMuzeumStart() {
/*
   * wysyłamy wiadomość do muzeum że jesteśmy gotowi oraz
   * czekamy na wiadomość od muzeum, że jest już gotowe
  */

  send_int(gMessageQueue, MUZEUM_POSTBOX, BANK_INITIALIZED, gF);

  InputStream_pointer response = MessageBuffer_get(gMessageBuffer);
  int type = InputStream_readChar(response);
  assert(type == MUZEUM_INITIALIZED);
  gDlugosc = InputStream_readRawInt(response);
  gGlebokosc = InputStream_readRawInt(response);
  InputStream_free(response);
}

void StartAllFirms() {
  for (size_t i = 0 ; i < gF ; i++)
    StartFirmProcess(&gFirmy[i]);
}

void SendShutDownToFirm(int Fid) {
  assert(0 < Fid && Fid <= gF);

  struct Firma* firma = &gFirmy[Fid - 1];
  if (firma->pid != 0) {
    Signal_sendSignalToProcess(firma->pid, SIGINT);
    DEBUG(puts("Bank - wysylam firmie sigint"));
  }

}

void CleanupBank() {
  MessageBuffer_killYourselfAndFree(gMessageBuffer);

  send_simple(gMessageQueue, MUZEUM_POSTBOX, BANK_READY_TO_DEATH);

  MessageQueue_free(gMessageQueue);
  gMessageQueue = NULL;
  free(gFirmy);
  gFirmy = NULL;
  gF = gS = gA = 0;
}

void StartFirmProcess(struct Firma * firma) {
  pid_t pid;
  String_pointer S = safe_int_to_string(gS);
  String_pointer A = safe_int_to_string(gA);
  String_pointer id = safe_int_to_string(firma->Fid);
  String_pointer k = safe_int_to_string(firma->Fk);
  switch (pid = fork()) {
    case -1: /* blad */
      syserr("bank - StartFirmProcess, error in fork");
    case 0: /* proces potomny */
      execl("./firma",
            "firma",
            String_toCstring(S),
            String_toCstring(A),
            String_toCstring(id),
            String_toCstring(k),
            NULL);
      syserr("bank - StartFirmProcess, error in execl");
  }

  firma->pid = pid;
  String_free(S);
  String_free(A);
  String_free(id);
  String_free(k);
}

void GetMySaldo(int who, int postbox) {
  assert(0 < who && who <= gF);

  StringBuilder_pointer message = StringBuilder_createEmpty();
  StringBuilder_appendRawInt(message, gFirmy[who - 1].Fsaldo);
  send(gMessageQueue, postbox, message);
  StringBuilder_free(message);
}

void SendMoney(int from, int to, int how_many, int postbox) {
  assert(how_many >= 0);
  assert(0 <= from && from <= gF);
  assert(0 <= to && to <= gF);
  assert(from != to);

  // w naszym zadaniu przelewy tylko muzeum-firma lub firma-muzeum
  assert(from == 0 || to == 0);

  if (from == 0) {
    gFirmy[to - 1].Fsaldo += how_many;
  }
  else {
    gFirmy[from - 1].Fsaldo -= how_many;
    assert(gFirmy[from - 1].Fsaldo >= 0);
  }

  String_pointer response = String_fromCString("OK");
  MessageQueue_send(gMessageQueue, postbox, response);
  String_free(response);
}

int gF = 0;
int gS = 0;
int gA = 0;
int gDlugosc = 0;
int gGlebokosc = 0;
struct Firma* gFirmy = NULL;
MessageQueue_pointer gMessageQueue = NULL;
MessageBuffer_pointer gMessageBuffer = NULL;
int gDeadFirms = 0;
/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "muzeum.h"
#include "tables_operations.h"
#include "delegate.h"

int main(int argc, char * argv[]) {
  InitializeMuzeum(argc, argv);
  DEBUG(puts("Muzeum - zainicjowano."));
  WaitForBankStartAndFinalizeInitialization();

  DEBUG(puts("Muzeum - rozpoczynam nasluchiwanie."));
  while (gRaportsCount < gF || !gBankIsDead) {
    InputStream_pointer message = MessageBuffer_get(gMessageBuffer);
    int type = InputStream_readChar(message);
    switch (type) {
      case MUZEUM_RAPORT_PART:
      {
        int Fid = InputStream_readRawInt(message);
        //DEBUG(printf("Muzeum - odebralem czesc raportu od firmy %d.\n", Fid));
        String_pointer text = InputStream_readAll(message);
        StringBuilder_appendString(gFirmInfo[Fid - 1].partial_report, text);
        String_free(text);
        break;
      }
      case MUZEUM_RAPORT_END:
      {
        int Fid = InputStream_readRawInt(message);
        DEBUG(printf("Muzeum - firma %d zakonczyla wysylanie raportu.\n", Fid));
        gFirmInfo[Fid - 1].report_done = true;
        gRaportsCount++;
        break;
      }
      case BANK_READY_TO_DEATH:
      {
        //DEBUG(puts("Muzeum - odebralem BANK_READY_TO_DEATH."));
        gBankIsDead = true;
        break;
      }
      case FIRM_REQUEST_FOR_PERMISSION:
      {
        int Fid = InputStream_readRawInt(message);
        int Fk = InputStream_readRawInt(message);
        int Z = InputStream_readRawInt(message);
        //DEBUG(printf("Muzeum - prosba o pozwolenie, Fid=%d, Fk=%d, Z=%d\n", Fid, Fk, Z));
        RequestForPermission(Fid, Fk, Z);
        break;
      }
      case MUZEUM_GET_ESTEEM:
      {
        int Fid = InputStream_readRawInt(message);
        int L = InputStream_readRawInt(message);
        int P = InputStream_readRawInt(message);
        int G = InputStream_readRawInt(message);
        //DEBUG(printf("Muzeum - prosba o szacowanie, Fid=%d, L=%d, P=%d, G=%d\n", Fid, L, P, G));
        GetEsteem(Fid, L, P, G);
        break;
      }
      case MUZEUM_SELL_ARTIFACTS:
      {
        int Fid = InputStream_readRawInt(message);
        int artifact = InputStream_readRawInt(message);
        int count = InputStream_readRawInt(message);
        if (artifact == count) {
          SendMoney(Fid, artifact * 10);
          send_simple(gMessageQueue, FIRM_POSTBOX(Fid), FIRM_SELL_OK);
        }
        else {
          send_simple(gMessageQueue, FIRM_POSTBOX(Fid), FIRM_SELL_NOT_OK);
        }

        break;
      }
      default:
        fatal("muzeum - reseived unknown message, %d", type);
    }

    InputStream_free(message);

    // sprawdzamy, czy przypadkiem wszystkie artefakty nie zostały wydobyte
    if (AllArtifactsExcavated()) {
      // wysyłamy do banku informację o tym, że ma zamknąć firmy i siebie
      send_simple(gMessageQueue, BANK_POSTBOX, BANK_SHUTDOWN);
    }
  }

  for (int Fid = 1 ; Fid <= gF ; Fid++)
    WaitForDelegateEnd(Fid);

  PrintMuzeumRaport();
  CleanupMuzeum();
  return 0;
}

void InitializeMuzeum(int argc, char ** argv) {
  if (argc != 5)
    fatal("muzeum - wrong number of arguments");
  gDlugosc = safe_cstring_to_long_min_max(argv[1], 1, LONG_MAX);
  gGlebokosc = safe_cstring_to_long_min_max(argv[2], 1, LONG_MAX);
  gS = safe_cstring_to_long_min_max(argv[3], 1, LONG_MAX);
  gA = safe_cstring_to_long_min_max(argv[4], 1, LONG_MAX);
  gTeren = (int**)safe_allocate_2d_array((size_t)gDlugosc, (size_t)gGlebokosc, sizeof(int));
  gSzacunek = (int**)safe_allocate_2d_array((size_t)gDlugosc, (size_t)gGlebokosc, sizeof(int));
  gReserved = (int*)safe_raw_allocate((size_t)gDlugosc, sizeof(int));
  gFirstNotExcavated = (int*)safe_raw_allocate((size_t)gDlugosc, sizeof(int));

  for (size_t i = 0 ; i < gDlugosc ; i++)
    for (size_t j = 0 ; j < gGlebokosc ; j++)
      scanf("%d", &gTeren[i][j]);

  for (size_t i = 0 ; i < gDlugosc ; i++)
    for (size_t j = 0 ; j < gGlebokosc ; j++)
      scanf("%d", &gSzacunek[i][j]);

  gMessageQueue = MessageQueue_openOrCreate(MESSAGE_QUEUE_KEY);
  gMessageBuffer = MessageBuffer_create(gMessageQueue, MUZEUM_POSTBOX);
  Signal_setHandler(SIGINT, SigintHandlerMuzeum);
}

void WaitForBankStartAndFinalizeInitialization() {
  /*
  * wysyłamy wiadomość do banku że jesteśmy gotowi oraz
  * czekamy na wiadomość od banku, że jest już gotowy
 */
  send_int_int(gMessageQueue, BANK_POSTBOX, MUZEUM_INITIALIZED, gDlugosc, gGlebokosc);

  InputStream_pointer response = MessageBuffer_get(gMessageBuffer);
  int type = InputStream_readChar(response);
  assert(type == BANK_INITIALIZED);
  gF = InputStream_readRawInt(response);
  InputStream_free(response);

  gFirmInfo = safe_raw_allocate((size_t)gF, sizeof(struct FirmInfo));
  for (size_t i = 0 ; i < gF ; i++) {
    gFirmInfo[i].partial_report = StringBuilder_createEmpty();
    gFirmInfo[i].report_done = false;
  }

}

void WaitForDelegateEnd(int Fid) {
  if (gFirmInfo[Fid - 1].delegate != NULL) {
    Thread_joinAndFree(gFirmInfo[Fid - 1].delegate, NULL);
    gFirmInfo[Fid - 1].delegate = NULL;
  }
}

void RequestForPermission(int Fid, int Fk, int Z) {
  assert(0 < Fid && Fid <= gF);
  //assert(gFirmInfo[Fid - 1].delegate == NULL);
  WaitForDelegateEnd(Fid);
  //puts("Muzeum - prosze o saldo.");
  int firm_saldo = GetSaldo(gMessageQueue, Fid, MUZEUM_ALTERNATIVE_POSTBOX);

  /*
   * to oczywiście nigdy się nie zdarzy, gdyż firmy wysyłają
   * zapytania z poprawnym hajsem
   * Trudno jednak powiedzieć, o co chodzi w treści więc warto
   * dopisać takie "życzeniowe" ify.
   */
  if (firm_saldo < Z) {
    TakeMoney(Fid, gS);
    RejectRequest(Fid);
    return;
  }

  int L, G;
  if (!FindSatisfyingArea(Z, Fk, &L, &G)) {
    TakeMoney(Fid, gS);
    RejectRequest(Fid);
    return;
  }

  SetRangeOccupied(L - 1, L - 1 + Fk, Fid);

  struct PermissionInfo* args = safe_raw_allocate(1, sizeof(struct PermissionInfo));
  args->Fid = Fid;
  args->G = G;
  args->L = L;
  args->Fk = Fk;
  gFirmInfo[Fid - 1].delegate = Thread_create(DelegateThread, args, true);

  TakeMoney(Fid, Z);
  AcceptRequest(Fid, L, G);
}

void RejectRequest(int Fid) {
  send_simple(gMessageQueue, FIRM_POSTBOX(Fid), MUZEUM_REQUEST_REJESTED);
}

void AcceptRequest(int Fid, int L, int G) {
  send_int_int(gMessageQueue, FIRM_POSTBOX(Fid), MUZEUM_REQUEST_ACCEPTED, L, G);
}

void TakeMoney(int Fid, int how_much) {
  { // przelew
    StringBuilder_pointer message = StringBuilder_createEmpty();
    StringBuilder_appendChar(message, BANK_SEND_MONEY);
    StringBuilder_appendRawInt(message, Fid);
    StringBuilder_appendRawInt(message, 0);
    StringBuilder_appendRawInt(message, how_much);
    StringBuilder_appendRawInt(message, MUZEUM_ALTERNATIVE_POSTBOX);
    send(gMessageQueue, BANK_POSTBOX, message);
    StringBuilder_free(message);
  }
  { // odbior potwierdzenia od banku
    InputStream_pointer response = receive(gMessageQueue, MUZEUM_ALTERNATIVE_POSTBOX);
    InputStream_free(response);
  }
}

void SendMoney(int Fid, int how_much) {
  { // przelew
    StringBuilder_pointer message = StringBuilder_createEmpty();
    StringBuilder_appendChar(message, BANK_SEND_MONEY);
    StringBuilder_appendRawInt(message, 0);
    StringBuilder_appendRawInt(message, Fid);
    StringBuilder_appendRawInt(message, how_much);
    StringBuilder_appendRawInt(message, MUZEUM_ALTERNATIVE_POSTBOX);
    send(gMessageQueue, BANK_POSTBOX, message);
    StringBuilder_free(message);
  }
  { // odbior potwierdzenia od banku
    InputStream_pointer response = receive(gMessageQueue, MUZEUM_ALTERNATIVE_POSTBOX);
    InputStream_free(response);
  }
}

void GetEsteem(int Fid, int L, int P, int G) {
  assert(sizeof(int) * 10 <= MESSAGE_MAX_SIZE);
  assert(0 < Fid && Fid <= gF);
  assert(0 < L && L <= P && P <= gDlugosc);
  assert(0 < G && G <= gGlebokosc);

  /**
   * Wysyłamy pakiety po 10 intów, odbiorca zna L, P i G
   * więc będzie wstanie wpisać to sobie do tablicy.
   */
  StringBuilder_pointer message = StringBuilder_createEmpty();
  int count = 0;

  for (int i = (L - 1) ; i < P ; i++) {
    for (int j = 0 ; j < G ; j++) {
      StringBuilder_appendRawInt(message, gSzacunek[i][j]);
      count++;

      if (count >= 10) {
        send(gMessageQueue, FIRM_POSTBOX(Fid), message);
        StringBuilder_free(message);
        message = StringBuilder_createEmpty();
        count = 0;
      }
    }
  }

  if (count > 0) {
    send(gMessageQueue, FIRM_POSTBOX(Fid), message);
    StringBuilder_free(message);
    message = StringBuilder_createEmpty();
    count = 0;
  }

  StringBuilder_free(message);
}

void PrintMuzeumRaport() {
  // stan wykopalisk
  /**
   * 1 nigdy nie zostanie wypisane, gdyż firma przed śmiercią zwalnia
   * wszystkie zasoby a więc również pozwolenia, muzeum przed śmiercią
   * musi wypisać raport museum a w nim raporty firm, by raport firmy
   * był wygenerowany firma musi umrzeć. Zatem przed wypisaniem raportu
   * museum wszystkie pozwolenia będą zwolnione.
   */

  for (size_t i = 0 ; i < gDlugosc ; i++) {
    for (size_t j = 0 ; j < gGlebokosc ; j++) {
      putchar((gSzacunek[i][j] == 0)? '0' : '2');
    }
    puts("");
  }

  // raporty firm
  for (size_t i = 0 ; i < gF ; i++) {
    String_pointer raportFirmy = StringBuilder_toString(gFirmInfo[i].partial_report);
    puts(String_toCstring(raportFirmy));
    puts("");
    String_free(raportFirmy);
  }
}

void CleanupMuzeum() {
  for (size_t i = 0 ; i < gF ; i++)
    StringBuilder_free(gFirmInfo[i].partial_report);
  free(gFirmInfo);
  gFirmInfo = NULL;

  MessageBuffer_killYourselfAndFree(gMessageBuffer);
  MessageQueue_closeAndFree(gMessageQueue);
  gMessageQueue = NULL;
  safe_deallocate_2d_array((void**)gSzacunek, (size_t )gDlugosc);
  gSzacunek = NULL;
  safe_deallocate_2d_array((void**)gTeren, (size_t )gDlugosc);
  gTeren = NULL;
  free(gReserved);
  gReserved = NULL;
  free(gFirstNotExcavated);
  gFirstNotExcavated = NULL;

  gDlugosc = 0;
  gGlebokosc = 0;
  gS = 0;
  gA = 0;
}

void SigintHandlerMuzeum(int n) {
  Signal_ignoreSignal(SIGINT);
  DEBUG(puts("Muzeum - tu sigint handler."));

  DEBUG(puts("Muzeum - wysylam do banku 'zabij sie'."));
  send_simple(gMessageQueue, BANK_POSTBOX, BANK_SHUTDOWN);
}

bool AllArtifactsExcavated() {
  for (size_t i = 0 ; i < gDlugosc ; i++)
    if (gFirstNotExcavated[i] < gGlebokosc)
      return false;
  return true;
}

int gDlugosc = 0;
int gGlebokosc = 0;
int* gReserved = NULL;
int gS = 0;
int gA = 0;
int gF = 0;
int** gTeren = NULL;
int* gFirstNotExcavated = NULL;
int** gSzacunek = NULL;
MessageQueue_pointer gMessageQueue = NULL;
MessageBuffer_pointer gMessageBuffer = NULL;
int gRaportsCount = 0;
struct FirmInfo * gFirmInfo = NULL;
bool gBankIsDead = false;
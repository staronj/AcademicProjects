/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_BANK_H
#define PROJECT_BANK_H

#include "../system_headers.h"
#include "../err.h"
#include "../safe_utility.h"
#include "../input_stream.h"
#include "../constants.h"
#include "../message_queue.h"
#include "../message_buffer.h"
#include "../signals.h"

/*
 * Globalne zmienne z prefiksem g.
 */
/**
 * Znaczenie jak w treści zadania.
 */
extern int gF;
extern int gS;
extern int gA;
extern int gDlugosc;
extern int gGlebokosc;

/**
 * Kolejka komunikatów oraz aktywna nakładka na nią.
 * (Parz message_buffer.h)
 */
extern MessageQueue_pointer gMessageQueue;
extern MessageBuffer_pointer gMessageBuffer;

/**
 * Zlicza, ile firm wysłało już do banku potwierdzenie swojej śmierci.
 */
extern int gDeadFirms;


struct Firma {
  pid_t pid;
  int Fid;
  int Fsaldo;
  int Fk;
};
extern struct Firma* gFirmy;

void InitializeBank(int argc, char ** argv);
void WaitForMuzeumStart();
void CleanupBank();
void StartFirmProcess(struct Firma * firma);
void StartAllFirms();

/**
 * Funkcje do obsługi zapytań.
 */
void GetMySaldo(int who, int postbox);
void SendMoney(int from, int to, int how_many, int postbox);

/**
 * Funkcja do informowania firmy, że powinna zakończyć działalność.
 */
void SendShutDownToFirm(int gFid);


#endif //PROJECT_BANK_H

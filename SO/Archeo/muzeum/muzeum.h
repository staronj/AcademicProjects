/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_MUZEUM_H_H
#define PROJECT_MUZEUM_H_H

#include "../constants.h"
#include "../err.h"
#include "../message_queue.h"
#include "../safe_utility.h"
#include "../string_builder.h"
#include "../signals.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "../utility.h"
#include "../message_buffer.h"
#include "../thread.h"

/**
 * Globalne zmienne z prefixem g.
 */
extern int gDlugosc;
extern int gGlebokosc;
extern int gS;
extern int gA;
extern int gF;
extern int** gTeren;
extern int* gFirstNotExcavated;
extern int** gSzacunek;
extern int* gReserved;
extern MessageQueue_pointer gMessageQueue;
extern MessageBuffer_pointer gMessageBuffer;
extern bool gBankIsDead;


struct FirmInfo {
  StringBuilder_pointer partial_report;
  bool report_done;

  Thread_pointer delegate;
};

extern int gRaportsCount;
extern struct FirmInfo * gFirmInfo;

void InitializeMuzeum(int argc, char ** argv);

void WaitForBankStartAndFinalizeInitialization();


void RequestForPermission(int Fid, int Fk, int Z);
void TakeMoney(int Fid, int how_much);
void SendMoney(int Fid, int how_much);
void RejectRequest(int Fid);
void AcceptRequest(int Fid, int L, int G);

void GetEsteem(int Fid, int L, int P, int G);

void WaitForDelegateEnd(int Fid);
void PrintMuzeumRaport();
void CleanupMuzeum();
void SigintHandlerMuzeum(int);

bool AllArtifactsExcavated();

#endif //PROJECT_MUZEUM_H_H

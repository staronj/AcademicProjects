/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_FIRMA_H
#define PROJECT_FIRMA_H

#include "../system_headers.h"
#include "../message_queue.h"
#include "../safe_utility.h"
#include "../thread.h"
#include "../utility.h"
#include "../constants.h"
#include "../string_builder.h"
#include "../message_buffer.h"
#include "../mutex_and_signal.h"
#include "../signals.h"
#include "activity_suspender.h"
#include "worker.h"

/**
 * Zmienne globalne z prefiksem g.
 */

/**
 * Znaczenie jak w treści.
 */
extern int gS;
extern int gA;
extern int gFid;
extern int gFk;

/**
 * gMessageQueue - kolejka komunikatów.
 *
 * gOwnedArtifacts - tablica z posiadanymi artefaktami
 *
 * gMutex - mutex do chronienia dostępu do zmiennych globalnych
 * zmienianych z wątków "Worker"
 *
 * gSigintReceived - zmienna globalna określająca, czy otrzymaliśmy już
 * sygnał sigint
 *
 * gActivitySuspender - monitor umożliwiający zawieszanie działalności przez firmę
 */
extern MessageQueue_pointer gMessageQueue;
extern int* gOwnedArtifacts;
extern MutexAndSignal_pointer gMutex;
extern bool gSigintReceived;
extern ActivitySuspender_pointer gActivitySuspender;

struct PermissionInfo {
  bool granted;

  int L, G;
};

/*
 * Zmienna globalna trzymająca informacje o akrualnym pozwoleniu
 * (lub jego braku)
 */
extern struct PermissionInfo gPermissionInfo;

void InitializeFirm(int argc, char ** argv);
void CleanupFirm();

void AskForWorkPermission(int Z);
void SellArtifacts();

void SigintHandlerFirma(int);
void SigusrHandlerFirma(int);

InputStream_pointer GenerateFirmReport();

#endif //PROJECT_FIRMA_H

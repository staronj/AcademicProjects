/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_TABLES_OPERATIONS_H
#define PROJECT_TABLES_OPERATIONS_H

#include "muzeum.h"

/**
 * Znajduje spójny obszar wydobycia jak w treści zadania.
 *
 * Nie o optymalne złożoności proszą nas w tym zadaniu, więc
 * nie przejmujemy się, że liczy się w koszmarnej.
 */
bool FindSatisfyingArea(int Z, int K, int* L, int* G);

/**
 * Operacje na gReserved
 */
bool IsRangeFree(int begin, int end);
void SetRangeFree(int begin, int end);
void SetRangeOccupied(int begin, int end, int Fid);

/**
 * Calculates gSzacunek[begin][G} + ... + gSzacunek[end - 1][G]
 */
long long GetRangeSumOnSzacunek(int begin, int end, int G);

#endif //PROJECT_TABLES_OPERATIONS_H

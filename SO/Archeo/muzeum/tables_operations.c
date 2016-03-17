/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "tables_operations.h"


bool FindSatisfyingArea(int Z, int K, int* L, int* G) {
  if (K > gDlugosc)
    return false;

  for (size_t i = 0 ; i < gDlugosc - K ; i++) {
    if (!IsRangeFree(i, i + K))
      continue;
    unsigned long long sum = 0;

    for (*G = 0 ; *G < gGlebokosc ; (*G)++) {
      long long next = GetRangeSumOnSzacunek(i, i + K, *G);

      if (sum + next >= (Z - gS))
        break;

      sum += next;
    }

    if (0 < sum && sum < (Z - gS)) {
      // kto wymyślił by numerować od 1? to C, nie jakiś pascal
      *L = i + 1;
      return true;
    }
  }
  return false;
}

bool IsRangeFree(int begin, int end) {
  assert(gReserved != NULL);
  assert(0 <= begin && begin < end && end < gDlugosc);
  for (int i = begin ; i < end ; i++) {
    if (gReserved[i])
      return false;
  }
  return true;
}

void SetRangeFree(int begin, int end) {
  assert(gReserved != NULL);
  assert(0 <= begin && begin < end && end < gDlugosc);
  for (int i = begin ; i < end ; i++) {
    assert(gReserved[i] != 0);
    gReserved[i] = 0;
  }
}

void SetRangeOccupied(int begin, int end, int Fid) {
  assert(gReserved != NULL);
  assert(0 <= begin && begin < end && end < gDlugosc);
  assert(Fid > 0);
  for (int i = begin ; i < end ; i++) {
    assert(gReserved[i] == 0);
    gReserved[i] = Fid;
  }
}

long long GetRangeSumOnSzacunek(int begin, int end, int G) {
  assert(gSzacunek != NULL);
  assert(0 <= begin && begin < end && end < gDlugosc);
  assert(0 <= G && G < gGlebokosc);

  long long sum = 0;
  for (int i = begin ; i < end ; i++) {
    sum += gSzacunek[i][G];
  }
  return sum;
}

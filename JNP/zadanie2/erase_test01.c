#include <assert.h>
#include <string.h>
#include "maptel.h"
#include <stddef.h>

static const char t1[] = "1";
static const char t2[] = "2";

int main() {
  unsigned long id;
  char tel[TEL_NUM_MAX_LEN + 1]; /* +1 na terminalne zero */

  id = maptel_create();
  maptel_insert(id, t1, t2);
  maptel_erase(id, t2);
  maptel_erase(id, t1);
  maptel_erase(id, t1);
  maptel_delete(id);
  return 0;
}


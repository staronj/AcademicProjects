#include <assert.h>
#include <string.h>
#include "maptel.h"
#include <stddef.h>

static const char invalid01[] = "abc";
static const char invalid02[] = "xyz";
static const char invalid03[] = "01234567891011121314151617181920";

int main() {
  unsigned long id;
  char tel[TEL_NUM_MAX_LEN + 1]; /* +1 na terminalne zero */

  id = maptel_create();
  maptel_insert(id, invalid01, invalid02);
  maptel_insert(id, invalid02, invalid03);
  maptel_delete(id);

  return 0;
}

#include <assert.h>
#include <string.h>
#include "maptel.h"
#include <stddef.h>

static const char t1[] = "1";
static const char t2[] = "2";

int main() {
    unsigned long id = maptel_create();
    maptel_insert(id, t1, t2);
    maptel_insert(id, t1, t2);
    return 0;
}



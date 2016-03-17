#include <assert.h>
#include <string.h>
#include "maptel.h"
#include <stddef.h>

static const char t1[] = "a";
static const char t2[] = "01234567890123456789123";

int main() {
    unsigned long id = maptel_create();
    maptel_insert(id, t1, t2);
    return 0;
}

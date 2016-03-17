#include <assert.h>
#include <string.h>
#include "maptel.h"
#include <stddef.h>

int main() {
    unsigned long id;

    for(int i = 0; i < 1000; i++) {
        id = maptel_create();
        assert(id == i);
    }

    for(int i = 0; i < 500; i++)
        maptel_delete(2 * i);

    for(int i = 0; i < 500; i++) {
        id = maptel_create();
        assert(id == 998 - 2 * i);
    }

  return 0;
}


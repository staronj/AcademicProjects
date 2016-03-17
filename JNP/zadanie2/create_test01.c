#include <assert.h>
#include <string.h>
#include "maptel.h"
#include <stddef.h>

int main() {
    unsigned long id;
    for(int i = 0; i < 1000000; i++) {
        id = maptel_create();
        assert(id == i);
    }
    return 0;
}


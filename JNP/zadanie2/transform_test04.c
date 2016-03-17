#include <assert.h>
#include <string.h>
#include "maptel.h"
#include <stddef.h>
#include <stdio.h>

static const char t1[] = "11111";
static const char t2[] = "2222";
static const char t3[] = "333";
static const char t4[] = "44";

int main() {
    unsigned long id = maptel_create();
    char tel[TEL_NUM_MAX_LEN + 1];
    maptel_insert(id, t1, t2);
    maptel_insert(id, t3, t4);
    maptel_transform(id, t1, tel, 5);
    printf("%s\n", tel);
    maptel_transform(id, t3, tel, 3);
    printf("%s\n", tel);
    return 0;
}


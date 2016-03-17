#include <assert.h>
#include <string.h>
#include "maptel.h"
#include <stddef.h>
#include <stdio.h>

static const char t1[] = "1";
static const char t2[] = "2";
static const char t3[] = "3";
static const char t4[] = "4";
static const char t5[] = "5";

int main() {
    unsigned long id = maptel_create();
    char tel[TEL_NUM_MAX_LEN + 1];
    maptel_insert(id, t1, t2);
    maptel_insert(id, t2, t3);
    maptel_insert(id, t3, t1);
    maptel_insert(id, t4, t1);
    maptel_insert(id, t5, t3);
    maptel_transform(id, t4, tel, TEL_NUM_MAX_LEN + 1);
    printf("%s\n", tel);
    maptel_transform(id, t5, tel, TEL_NUM_MAX_LEN + 1);
    printf("%s\n", tel);
    maptel_erase(id, t3);
    maptel_transform(id, t5, tel, TEL_NUM_MAX_LEN + 1);
    printf("%s\n", tel);
    maptel_transform(id, t4, tel, TEL_NUM_MAX_LEN + 1);
    printf("%s\n", tel);
    return 0;
}

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
static const char t6[] = "6";

int main() {
    char tel[TEL_NUM_MAX_LEN + 1];
    maptel_create();
    maptel_create();
    maptel_create();
    maptel_delete(1);
    maptel_insert(0, t1, t2);
    maptel_insert(0, t2, t3);
    maptel_insert(0, t3, t4);
    maptel_insert(0, t4, t1);
    maptel_insert(0, t5, t6);
    maptel_insert(2, t6, t5);
    maptel_insert(2, t5, t1);
    maptel_transform(0, t1, tel, 2);
    printf("%s\n", tel);
    maptel_transform(0, t2, tel, 2);
    printf("%s\n", tel);
    maptel_transform(0, t3, tel, 2);
    printf("%s\n", tel);
    maptel_transform(0, t4, tel, 2);
    printf("%s\n", tel);
    maptel_transform(0, t5, tel, 2);
    printf("%s\n", tel);
    maptel_transform(0, t6, tel, 2);
    printf("%s\n", tel);
    maptel_transform(2, t1, tel, 2);
    printf("%s\n", tel);
    maptel_transform(2, t2, tel, 2);
    printf("%s\n", tel);
    maptel_transform(2, t3, tel, 2);
    printf("%s\n", tel);
    maptel_transform(2, t4, tel, 2);
    printf("%s\n", tel);
    maptel_transform(2, t5, tel, 2);
    printf("%s\n", tel);
    maptel_transform(2, t6, tel, 2);
    printf("%s\n", tel);
    maptel_delete(0);
    maptel_delete(2);
    return 0;
}


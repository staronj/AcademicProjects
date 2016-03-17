/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "better_string.h"
#include "safe_memory_operations.h"
#include <malloc.h>
#include <assert.h>
#include <string.h>

struct String {
  char * data;
  size_t size;
};

static size_t min(size_t a, size_t b) {
  return (a < b)? a : b;
}

static String_pointer String_allocate() {
  return safe_raw_allocate(1, sizeof(struct String));
}

String_pointer String_fromData(const char *data, size_t size) {
  assert(data != NULL || size == 0);
  String_pointer result = String_allocate();
  // one more for null terminating character
  result->data = safe_char_allocate(size + 1);
  result->size = size;
  safe_char_copy(result->data, data, size);
  return result;
}

String_pointer String_fromCString(const char *c_string) {
  assert(c_string != NULL);
  size_t length = strlen(c_string);
  return String_fromData(c_string, length);
}

String_pointer String_clone(String_pointer string) {
  assert(string != NULL);
  return String_fromData(string->data, string->size);
}

void String_free(String_pointer this) {
  assert(this != NULL);
  free(this->data);
  this->data = NULL;
  this->size = 0;
  free(this);
}

String_pointer String_substr(String_pointer this, size_t begin, size_t count) {
  assert(this != NULL);
  assert(0 <= begin && begin <= this->size);
  size_t length = min(count, this->size - begin);
  return String_fromData(this->data + begin, length);
}

bool String_isEmpty(String_pointer this) {
  assert(this != NULL);
  return this->size == 0;
}

size_t String_size(String_pointer this) {
  assert(this != NULL);
  return this->size;
}

int String_compare(String_pointer a, String_pointer b) {
  assert(a != NULL && b != NULL);
  if (a->size != b->size)
    return (a->size < b->size)? -1 : 1;
  else
    return memcmp(a->data, b->data, a->size);
}

int String_compareCstr(String_pointer a, const char* b) {
  String_pointer tmp = String_fromCString(b);
  int result = String_compare(a, tmp);
  String_free(tmp);
  return result;
}

String_pointer String_join(String_pointer a, String_pointer b) {
  assert(a != NULL && b != NULL);
  String_pointer result = String_allocate();
  result->data = safe_char_allocate(a->size + b->size + 1);
  result->size = a->size + b->size;
  safe_char_copy(result->data, a->data, a->size);
  safe_char_copy(result->data + a->size, b->data, b->size);
  return result;
}

const char * String_toCstring(String_pointer this) {
  return String_data(this);
}

char String_at(String_pointer this, size_t index) {
  assert(this != NULL);
  assert(index < this->size);
  return this->data[index];
}

const char * String_data(String_pointer this) {
  assert(this != NULL);
  return this->data;
}
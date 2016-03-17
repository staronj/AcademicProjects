/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "string_builder.h"
#include "safe_memory_operations.h"
#include "safe_utility.h"
#include "vector.h"
#include <assert.h>
#include <string.h>

struct StringBuilder {
  Vector_pointer vector;
};

static StringBuilder_pointer StringBuilder_allocate() {
  return safe_raw_allocate(1, sizeof(struct StringBuilder));
}

StringBuilder_pointer StringBuilder_createEmpty() {
  StringBuilder_pointer result = StringBuilder_allocate();
  result->vector = Vector_new(sizeof(char), VECTOR_DEFAULT);
  return result;
}

StringBuilder_pointer StringBuilder_fromString(String_pointer string) {
  const char * data = String_toCstring(string);
  size_t length = String_size(string);
  return StringBuilder_fromData(data, length);
}

StringBuilder_pointer StringBuilder_fromData(const char * data, size_t size) {
  StringBuilder_pointer result = StringBuilder_createEmpty();
  StringBuilder_appendData(result, data, size);
  return result;
}

StringBuilder_pointer StringBuilder_fromCString(const char * c_string) {
  assert(c_string != NULL);
  size_t length = strlen(c_string);
  return StringBuilder_fromData(c_string, length);
}

void StringBuilder_free(StringBuilder_pointer this) {
  assert(this != NULL);
  Vector_free(this->vector);
  free(this);
}

void StringBuilder_reserve(StringBuilder_pointer this, size_t target_capacity) {
  assert(this != NULL);
  Vector_reserve(this->vector, target_capacity);
}

void StringBuilder_clear(StringBuilder_pointer this) {
  assert(this != NULL);
  Vector_clear(this->vector);
}

String_pointer StringBuilder_toString(StringBuilder_const_pointer this) {
  assert(this != NULL);
  const char * data = Vector_data(this->vector);
  size_t size = Vector_size(this->vector);
  return String_fromData(data, size);
}

void StringBuilder_appendChar(StringBuilder_pointer this, char c) {
  assert(this != NULL);
  Vector_pushBack(this->vector, &c);
}

void StringBuilder_appendCString(StringBuilder_pointer this, const char * c_string) {
  assert(this != NULL);
  size_t length = strlen(c_string);
  StringBuilder_appendData(this, c_string, length);
}

void StringBuilder_appendData(StringBuilder_pointer this, const char * data, size_t size) {
  assert(this != NULL);
  Vector_append(this->vector, data, size);
}

void StringBuilder_appendString(StringBuilder_pointer this, String_pointer string) {
  assert(this != NULL);
  const char * data = String_toCstring(string);
  size_t size = String_size(string);
  StringBuilder_appendData(this, data, size);
}

void StringBuilder_insertChar(StringBuilder_pointer this, size_t position, char c) {
  assert(this != NULL);
  Vector_insert(this->vector, &c, position);
}

void StringBuilder_insertCString(StringBuilder_pointer this, size_t position, const char * c_string) {
  assert(this != NULL);
  size_t length = strlen(c_string);
  StringBuilder_insertData(this, position, c_string, length);
}

void StringBuilder_insertData(StringBuilder_pointer this, size_t position, const char * data, size_t size) {
  assert(this != NULL);
  Vector_insertN(this->vector, data, position, size);
}

void StringBuilder_insertString(StringBuilder_pointer this, size_t position, String_pointer string) {
  assert(this != NULL);
  const char * data = String_toCstring(string);
  size_t size = String_size(string);
  Vector_insertN(this->vector, data, position, size);
}

void StringBuilder_appendRawInt(StringBuilder_pointer this, int n) {
  StringBuilder_appendData(this, (char*)&n, sizeof(int));
}

void StringBuilder_appendFormattedInt(StringBuilder_pointer this, int n) {
  assert(this != NULL);
  String_pointer number = safe_int_to_string(n);
  StringBuilder_appendString(this, number);
  String_free(number);
}

char * StringBuilder_data(StringBuilder_pointer this) {
  assert(this != NULL);
  return Vector_data(this->vector);
}

size_t StringBuilder_size(StringBuilder_const_pointer this) {
  assert(this != NULL);
  return Vector_size(this->vector);
}

bool StringBuilder_empty(StringBuilder_const_pointer this) {
  assert(this != NULL);
  return Vector_empty(this->vector);
}

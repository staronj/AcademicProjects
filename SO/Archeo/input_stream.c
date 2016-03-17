/**
 * Copyrights Jakub Staroń 2015
 * numer indeksu 361025
 */

#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "input_stream.h"

struct InputStream {
  String_pointer string;
  size_t i;
};

static InputStream_pointer InputStream_allocate() {
  InputStream_pointer result = malloc(sizeof(struct InputStream));
  memset(result, 0, sizeof(struct InputStream));
  return result;
}

InputStream_pointer InputStream_fromCString(const char * c_string) {
  InputStream_pointer result = InputStream_allocate();
  result->i = 0;
  result->string = String_fromCString(c_string);
  return result;
}

InputStream_pointer InputStream_fromString(String_pointer string) {
  InputStream_pointer result = InputStream_allocate();
  result->i = 0;
  result->string = String_clone(string);
  return result;
}

InputStream_pointer InputStream_fromData(const char * data, size_t size) {
  InputStream_pointer result = InputStream_allocate();
  result->i = 0;
  result->string = String_fromData(data, size);
  return result;
}

void InputStream_free(InputStream_pointer this) {
  assert(this != NULL);
  String_free(this->string);
  this->string = NULL;
  this->i = 0;
  free(this);
}

char InputStream_readChar(InputStream_pointer this) {
  assert(this != NULL);
  assert(InputStream_hasNext(this));
  char c = InputStream_peekChar(this);
  this->i++;
  return c;
}

char InputStream_peekChar(InputStream_pointer this) {
  assert(this != NULL);
  assert(InputStream_hasNext(this));
  return String_at(this->string, this->i);
}

void InputStream_readData(InputStream_pointer this, void * output, size_t size) {
  assert(this != NULL);
  assert(InputStream_howManyMore(this) >= size);

  char* ptr = (char*)output;
  for (size_t i = 0 ; i < size ; i++)
    *ptr++ = InputStream_readChar(this);
}

String_pointer InputStream_readAll(InputStream_pointer this) {
  assert(this != NULL);
  String_pointer result = String_substr(this->string, this->i, String_size(this->string));
  this->i = String_size(this->string);
  return result;
}

String_pointer InputStream_readAtMost(InputStream_pointer this, size_t size) {
  assert(this != NULL);
  size_t how_many = InputStream_howManyMore(this);
  if (how_many < size) size = how_many;

  const char* data = String_data(this->string) + this->i;
  String_pointer result = String_fromData(data, how_many);
  this->i += how_many;

  return result;
}

int InputStream_readRawInt(InputStream_pointer this) {
  assert(this != NULL);
  int result = 0;
  InputStream_readData(this, &result, sizeof(int));
  return result;
}

bool InputStream_hasNext(InputStream_pointer this) {
  assert(this != NULL);
  return this->i < String_size(this->string);
}

size_t InputStream_howManyMore(InputStream_pointer this) {
  assert(this != NULL);
  return String_size(this->string) - this->i;
}

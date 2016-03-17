/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "safe_memory_operations.h"

#include <string.h>
#include <malloc.h>

void safe_raw_copy(void* destination, const void* source, size_t number, size_t sizeOfElement) {
  if(number > 0)
    memcpy(destination, source, number * sizeOfElement);
}

void* safe_raw_allocate(size_t number, size_t sizeOfElement) {
  if (number == 0) {
    return NULL;
  }
  else {
    size_t size = number * sizeOfElement;
    void* data = malloc(size);
    memset(data, 0, size);
    return data;
  }
}

void* safe_raw_offset(void* pointer, ptrdiff_t offset, size_t elementSize) {
  return (void*)((char*)(pointer) + offset * elementSize);
}

void safe_raw_move(void* data, size_t size, ptrdiff_t offset, size_t elementSize) {
  void* new_position = safe_raw_offset(data, offset, elementSize);
  memmove(new_position, data, size * elementSize);
}

void safe_char_copy(char* destination, const char* source, size_t number) {
  safe_raw_copy(destination, source, number, sizeof(char));
}

char* safe_char_allocate(size_t number) {
  return safe_raw_allocate(number, sizeof(char));
}

void safe_char_move(char* data, size_t size, ptrdiff_t offset) {
  safe_raw_move(data, size, offset, sizeof(char));
}

void safe_zero_memory(void* pointer, size_t number, size_t sizeOfElement) {
  memset(pointer, 0, number * sizeOfElement);
}
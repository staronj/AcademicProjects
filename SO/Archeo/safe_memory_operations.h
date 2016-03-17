/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef _SAFE_MEMORY_OPERATIONS_H
#define _SAFE_MEMORY_OPERATIONS_H

#include <stddef.h>

/**
 * Copy memory from source to destination.
 * If memory blocks can overlap, use move instead.
 */
void safe_raw_copy(void* destination, const void* source, size_t number, size_t sizeOfElement);

/**
 * Allocates memory and fill it with zeros.
 */
void* safe_raw_allocate(size_t number, size_t sizeOfElement);

/**
 * Save incrementation of void pointer.
 */
void* safe_raw_offset(void* pointer, ptrdiff_t offset, size_t elementSize);

/**
 * Move memory. Can be slower than copy.
 */
void safe_raw_move(void* data, size_t size, ptrdiff_t offset, size_t elementSize);

/**
 * Versions for char.
 */
void safe_char_copy(char* destination, const char* source, size_t number);

char* safe_char_allocate(size_t number);

void safe_char_move(char* data, size_t size, ptrdiff_t offset);

void safe_zero_memory(void* pointer, size_t number, size_t sizeOfElement);

#endif //_SAFE_MEMORY_OPERATIONS_H

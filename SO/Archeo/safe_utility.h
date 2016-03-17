/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_SAFE_UTILITY_H
#define PROJECT_SAFE_UTILITY_H

#include <stddef.h>
#include "safe_memory_operations.h"
#include "better_string.h"
#include "err.h"

/**
 * Performs safe conversion from c string to integer.
 * If fails, fail function is called.
 * Second version have additional parameters to asser integer range.
 */
long safe_cstring_to_long(const char*);
long safe_cstring_to_long_min_max(const char*, long, long);

/**
 * Allocates and deallocates 2d arrays.
 */
void** safe_allocate_2d_array(size_t first_size, size_t second_size, size_t element_size);
void safe_deallocate_2d_array(void** pointer, size_t first_size);

/**
 * Converts int to String
 */
String_pointer safe_int_to_string(int);

#endif //PROJECT_SAFE_UTILITY_H

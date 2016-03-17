/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef _BETTER_STRING_H
#define _BETTER_STRING_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Structure for immutable string.
 */
struct String;
typedef struct String * String_pointer;
typedef const struct String * String_const_pointer;

/**
 * Construct new string from data, c string or another string. Returned pointer points to
 * string structure. It should be freed using string_free.
 */
String_pointer String_fromData(const char * data, size_t size);
String_pointer String_fromCString(const char * c_string);
String_pointer String_clone(String_pointer string);

/**
 * Our destructor.
 */
void String_free(String_pointer this);

/**
 * Returns new string which will be substring of exising string.
 * Substring is count characters long,
 * or shorter if the end of the string comes first.
 */
String_pointer String_substr(String_pointer this, size_t begin, size_t count);

bool String_isEmpty(String_pointer this);
size_t String_size(String_pointer this);

/**
 * Compares two strings.
 * Shorter string is always less.
 * If strings have the same length, they are compared
 * by memcmp.
 */
int String_compare(String_pointer a, String_pointer b);
int String_compareCstr(String_pointer a, const char*);

/**
 * Returns concatenation of two strings.
 */
String_pointer String_join(String_pointer a, String_pointer b);

/**
 * Returns pointer to c string which is holding inside struct.
 * Pointer is valid as long as struct this.
 */
const char * String_toCstring(String_pointer this);

/**
 * Getter
 */
char String_at(String_pointer this, size_t index);

/**
 * Returns pointer to c string which is holding inside struct.
 * Pointer is valid as long as struct this.
 */
const char * String_data(String_pointer this);

#endif // _BETTER_STRING_H

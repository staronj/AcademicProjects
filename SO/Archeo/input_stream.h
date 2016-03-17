/**
 * Copyrights Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef _INPUT_STREAM_H
#define _INPUT_STREAM_H

#include <stdbool.h>
#include <stddef.h>
#include "better_string.h"

struct InputStream;
typedef struct InputStream * InputStream_pointer;

InputStream_pointer InputStream_fromCString(const char * c_string);
InputStream_pointer InputStream_fromString(String_pointer string);
InputStream_pointer InputStream_fromData(const char * data, size_t size);

void InputStream_free(InputStream_pointer this);

char InputStream_readChar(InputStream_pointer this);
char InputStream_peekChar(InputStream_pointer this);
void InputStream_readData(InputStream_pointer this, void * output, size_t size);
String_pointer InputStream_readAll(InputStream_pointer this);
String_pointer InputStream_readAtMost(InputStream_pointer this, size_t size);
int InputStream_readRawInt(InputStream_pointer this);

bool InputStream_hasNext(InputStream_pointer this);
size_t InputStream_howManyMore(InputStream_pointer this);

#endif //_INPUT_STREAM_H

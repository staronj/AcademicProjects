/**
 * Copyrights Jakub Staroñ 2015
 * numer indeksu 361025
 */

#ifndef _PROCESS_HANDLE_H
#define _PROCESS_HANDLE_H

#include <stdbool.h>
#include <stddef.h>
#include "better_string.h"

struct ProcessHandle;
typedef struct ProcessHandle * ProcessHandle_pointer;

ProcessHandle_pointer ProcessHandle_forkMe();

ProcessHandle_pointer ProcessHandle_fromDescriptors(int read_descriptor, int write_descriptor);


void ProcessHandle_free(ProcessHandle_pointer this);

bool ProcessHandle_amIChild(ProcessHandle_pointer this);

void ProcessHandle_writeString(ProcessHandle_pointer this, String_pointer string);
String_pointer ProcessHandle_readString(ProcessHandle_pointer this);
void ProcessHandle_closeRead(ProcessHandle_pointer this);
void ProcessHandle_closeWrite(ProcessHandle_pointer this);
void ProcessHandle_wait(ProcessHandle_pointer this);
void ProcessHandle_free(ProcessHandle_pointer this);

#endif //_PROCESS_HANDLE_H

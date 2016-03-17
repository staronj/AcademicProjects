/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_MESSAGE_QUEUE_H
#define PROJECT_MESSAGE_QUEUE_H

#include "system_headers.h"
#include "better_string.h"

struct MessageQueue;
typedef struct MessageQueue * MessageQueue_pointer;

#define MESSAGE_MAX_SIZE (100)

MessageQueue_pointer MessageQueue_openOrCreate(key_t key);
MessageQueue_pointer MessageQueue_open(key_t key);
void MessageQueue_send(MessageQueue_pointer this, long type, String_pointer string);
String_pointer MessageQueue_receive(MessageQueue_pointer this, long type);
void MessageQueue_closeAndFree(MessageQueue_pointer this);
void MessageQueue_free(MessageQueue_pointer this);

#endif //PROJECT_MESSAGE_QUEUE_H

/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "message_queue.h"
#include "safe_memory_operations.h"
#include "err.h"


/**
 * Errno is thread-safe, see
 * https://stackoverflow.com/questions/1694164/is-errno-thread-safe
 */
#define HANDLE_EINR(call, call_name) \
while(true) { \
int result = call; \
if(result == -1 && errno == EINTR) { \
  continue; \
} else if (result == -1) { \
syserr(#call_name" error"); \
} \
else { \
break; \
} \
} \

struct MessageQueue {
  key_t key;
  int message_queue_handle;
};

struct MessageBuffer {
  long    message_type;
  size_t  message_size;
  char    message[MESSAGE_MAX_SIZE];
};

static MessageQueue_pointer create_queue_with_flags(key_t key, int flags) {
  flags |= 0666;
  int message_queue_handle = msgget(key, flags);

  if (message_queue_handle == -1) {
    syserr("msgget error");
  }

  MessageQueue_pointer result = safe_raw_allocate(1, sizeof(struct MessageQueue));
  result->key = key;
  result->message_queue_handle = message_queue_handle;
  return result;
}

MessageQueue_pointer MessageQueue_openOrCreate(key_t key) {
  return create_queue_with_flags(key, IPC_CREAT);
}

MessageQueue_pointer MessageQueue_open(key_t key) {
  return create_queue_with_flags(key, 0);
}

void MessageQueue_send(MessageQueue_pointer this, long type,
                       String_pointer string) {
  assert(this != NULL);
  assert(String_size(string) <= MESSAGE_MAX_SIZE);
  struct MessageBuffer buffer;
  safe_zero_memory(&buffer, 1, sizeof(buffer));
  buffer.message_type = type;
  buffer.message_size = String_size(string);
  safe_char_copy(buffer.message, String_data(string), String_size(string));

  int message_size = sizeof(struct MessageBuffer) - sizeof(long);
  HANDLE_EINR(msgsnd(this->message_queue_handle, (struct msgbuf *)&buffer, message_size, 0), msgsnd);
}

String_pointer MessageQueue_receive(MessageQueue_pointer this, long type) {
  assert(this != NULL);
  struct MessageBuffer buffer;

  int message_size = sizeof(struct MessageBuffer) - sizeof(long);
  HANDLE_EINR(msgrcv(this->message_queue_handle, (struct msgbuf *)&buffer, message_size, type, 0), msgrcv);
  return String_fromData(buffer.message, buffer.message_size);
}

void MessageQueue_closeAndFree(MessageQueue_pointer this) {
  assert(this != NULL);
  if (msgctl(this->message_queue_handle, IPC_RMID, 0) == -1)
    syserr("msgctl in remove_queue");

  MessageQueue_free(this);
}

void MessageQueue_free(MessageQueue_pointer this) {
  free(this);
}

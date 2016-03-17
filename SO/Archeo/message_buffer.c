/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "constants.h"
#include "message_buffer.h"
#include "safe_memory_operations.h"
#include "thread.h"
#include "utility.h"
#include "mutex_and_signal.h"
#include "err.h"

struct Node {
  InputStream_pointer message;
  struct Node* next;
};
typedef struct Node* Node_pointer;

static Node_pointer Node_create(InputStream_pointer message) {
  Node_pointer new = safe_raw_allocate(1, sizeof(struct Node));
  new->message = message;
  new->next = NULL;
  return new;
}

static void Node_free(Node_pointer this) {
  assert(this != NULL);
  free(this);
}

static void* Deamon(void*);

struct MessageBuffer {
  MessageQueue_pointer message_queue;
  int postbox;
  Node_pointer first;
  Node_pointer last;


  MutexAndSignal_pointer mutex_signal;
  Thread_pointer deamon;
};

static void node_put(MessageBuffer_pointer this, Node_pointer new_node) {
  assert((this->first == NULL) == (this->last == NULL));
  if (this->last == NULL) {
    this->first = this->last = new_node;
  }
  else {
    this->last->next = new_node;
    this->last = new_node;
  }
}

static Node_pointer node_get(MessageBuffer_pointer this) {
  assert(this->first != NULL);
  assert((this->first == NULL) == (this->last == NULL));

  if (this->first == this->last)
    this->last = NULL;

  Node_pointer old_first = this->first;
  this->first = old_first->next;

  assert((this->first == NULL) == (this->last == NULL));
  return old_first;
}

MessageBuffer_pointer MessageBuffer_create(MessageQueue_pointer queue, int postbox) {
  MessageBuffer_pointer this = safe_raw_allocate(1, sizeof(struct MessageBuffer));

  this->message_queue = queue;
  this->postbox = postbox;
  this->first = this->last = NULL;
  this->mutex_signal = MutexAndSignal_create();
  this->deamon = Thread_create(Deamon, this, true);
  return this;
}

InputStream_pointer MessageBuffer_get(MessageBuffer_pointer this) {
  assert(this != NULL);
  MutexAndSignal_lock(this->mutex_signal);

  while (this->first == NULL)
    MutexAndSignal_wait(this->mutex_signal);


  Node_pointer old_first = node_get(this);
  InputStream_pointer result = old_first->message;
  Node_free(old_first);

  MutexAndSignal_unlock(this->mutex_signal);
  return result;
}

void MessageBuffer_killYourselfAndFree(MessageBuffer_pointer this) {
  assert(this != NULL);

  send_simple(this->message_queue, this->postbox, MESSAGE_BUFFER_DEAMON_KILL_YOURSELF);

  // czekamy na śmierć deamona
  Thread_joinAndFree(this->deamon, NULL);

  // zwalniamy mytexy i sygnały
  MutexAndSignal_free(this->mutex_signal);

  // czyścimy kolejkę
  while (this->first != NULL) {
    Node_pointer node = node_get(this);
    InputStream_free(node->message);
    Node_free(node);
  }

  free(this);
}

static void* Deamon(void* this_pointer) {
  MessageBuffer_pointer this = (MessageBuffer_pointer)this_pointer;
  bool shutdown = false;

  while (!shutdown) {
    InputStream_pointer message = receive(this->message_queue, this->postbox);

    if (!InputStream_hasNext(message)) {
      fatal("MessageBuffer_Deamon - shit happened, read empty message.");
      InputStream_free(message);
    }
    else if (InputStream_peekChar(message) == MESSAGE_BUFFER_DEAMON_KILL_YOURSELF) {
      shutdown = true;
      InputStream_free(message);
    }
    else {
      MutexAndSignal_lock(this->mutex_signal);

      // w tym przypadku nie zwalniamy message, gdyż przekazujemy ją dalej
      Node_pointer new_node = Node_create(message);
      node_put(this, new_node);

      MutexAndSignal_signal(this->mutex_signal);
      MutexAndSignal_unlock(this->mutex_signal);
    }
  }

  return NULL;
}
/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#include "utility.h"

InputStream_pointer receive(MessageQueue_pointer message_queue, long postbox) {
  String_pointer string = MessageQueue_receive(message_queue, postbox);
  InputStream_pointer stream = InputStream_fromString(string);
  String_free(string);
  return stream;
}

void send(MessageQueue_pointer message_queue, long postbox, StringBuilder_pointer string_builder) {
  String_pointer string = StringBuilder_toString(string_builder);
  MessageQueue_send(message_queue, postbox, string);
  String_free(string);
}

void send_simple(MessageQueue_pointer message_queue, long postbox, char code) {
  StringBuilder_pointer message = StringBuilder_createEmpty();
  StringBuilder_appendChar(message, code);
  send( message_queue, postbox, message);
  StringBuilder_free(message);
}

void send_int(MessageQueue_pointer message_queue, long postbox, char code, int arg1) {
  StringBuilder_pointer message = StringBuilder_createEmpty();
  StringBuilder_appendChar(message, code);
  StringBuilder_appendRawInt(message, arg1);
  send( message_queue, postbox, message);
  StringBuilder_free(message);
}

void send_int_int(MessageQueue_pointer message_queue, long postbox, char code, int arg1, int arg2) {
  StringBuilder_pointer message = StringBuilder_createEmpty();
  StringBuilder_appendChar(message, code);
  StringBuilder_appendRawInt(message, arg1);
  StringBuilder_appendRawInt(message, arg2);
  send( message_queue, postbox, message);
  StringBuilder_free(message);
}

int GetSaldo(MessageQueue_pointer message_queue, int Fid, int postbox) {
  send_int_int(message_queue, BANK_POSTBOX, BANK_GET_SALDO, Fid, postbox);
  InputStream_pointer response = receive(message_queue, postbox);
  int result = InputStream_readRawInt(response);
  InputStream_free(response);
  return result;
}

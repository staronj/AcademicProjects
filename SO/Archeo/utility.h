/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_UTILITY_H
#define PROJECT_UTILITY_H

#include "message_queue.h"
#include "better_string.h"
#include "input_stream.h"
#include "string_builder.h"
#include "constants.h"

/**
 * Odbiera wiadomość z kolejki message_queue i typu postbox
 * Umieszcza ją w InputStream. Wywołujący jest odpowiedzialny
 * za zwolnienie InputStream.
 */
InputStream_pointer receive(MessageQueue_pointer message_queue, long postbox);

/**
 * Wysyła wiadomość na kolejkę message_queue i typu postbox.
 * Wiadomość jest przyjmowana jako obiekt StringBuilder. Funkcja nie
 * zwalnia tego obiektu!
 */
void send(MessageQueue_pointer message_queue, long postbox, StringBuilder_pointer string_builder);

/**
 * Wysyła prostą wiadomość zawierającą pojedyńczy znak char (interpretowany w tym
 * systemie jako wewnętrzny typ wiadomości)
 */
void send_simple(MessageQueue_pointer message_queue, long postbox, char code);

/**
 * Funkcje upraszczające wysyłanie wiadomości postaci (kod, int) albo (kod, int, int)
 */

void send_int(MessageQueue_pointer message_queue, long postbox, char code, int arg1);
void send_int_int(MessageQueue_pointer message_queue, long postbox, char code, int arg1, int arg2);

/**
 * Wysyła do banku zapytanie o saldo, prosi o odpowiedź na postbox
 */
int GetSaldo(MessageQueue_pointer message_queue, int Fid, int postbox);

#define DEBUG(x)

#endif //PROJECT_UTILITY_H

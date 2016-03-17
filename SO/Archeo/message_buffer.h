/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_MESSAGESBUFFER_H
#define PROJECT_MESSAGESBUFFER_H

#include "input_stream.h"
#include "message_queue.h"

/**
 * MessagesBuffer jest monitorem.
 * Instancja klasy tworzy wątek, który nasłuchuje na określonym
 * postboxie (typie kominukatu) kolejki, odbiera te komunikaty i
 * gromadzi w wewnętrznej kolejce.
 *
 * W tym samym czasie inny wątek może prosić o zgromadzone
 * komunikaty funkcją _get. Jeśli żaden komunikat nie znajduje się
 * w kolejce wątek będzie czekał.
 */

struct MessageBuffer;
typedef struct MessageBuffer* MessageBuffer_pointer;

/**
 * Tworzy nowy MessageBuffer który nasłuchuje na kolejce queue na typie postbox
 */
MessageBuffer_pointer MessageBuffer_create(MessageQueue_pointer queue, int postbox);

/**
 * Wysiąga jedną wiadomość z kolejki. Jeśli nie ma, czeka na takową.
 */
InputStream_pointer MessageBuffer_get(MessageBuffer_pointer this);

/**
 * Zatrzymuje deamona i zwalnia zasoby. Jeśli ktoś czeka na _get to
 * zachowanie niezdefiniowane...
 */
void MessageBuffer_killYourselfAndFree(MessageBuffer_pointer this);

#endif //PROJECT_MESSAGESBUFFER_H

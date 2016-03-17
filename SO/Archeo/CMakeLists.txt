set(CMAKE_C_FLAGS "-std=c99 -Wall -g -D_SVID_SOURCE=1 -D_XOPEN_SOURCE=700 -pthread")

set(SOURCES better_string.c better_string.h string_builder.c string_builder.h input_stream.c input_stream.h err.h err.c vector.h vector.c safe_memory_operations.c safe_memory_operations.h message_queue.c message_queue.h thread.c thread.h safe_utility.c safe_utility.h constants.h utility.c utility.h message_buffer.c message_buffer.h mutex_and_signal.c mutex_and_signal.h signals.c signals.h system_headers.h)

add_executable (muzeum muzeum/muzeum.c muzeum/muzeum.h ${SOURCES} muzeum/tables_operations.c muzeum/tables_operations.h muzeum/delegate.c muzeum/delegate.h)
add_executable (bank bank/bank.c bank/bank.h ${SOURCES})
add_executable (firma ${SOURCES} firma/firma.c firma/firma.h firma/worker.c firma/worker.h firma/activity_suspender.c firma/activity_suspender.h)
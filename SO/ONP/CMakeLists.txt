set(CMAKE_C_FLAGS "-std=c99 -Wall -g")

set(SOURCES better_string.c better_string.h string_builder.c string_builder.h  process_handle.c process_handle.h input_stream.c input_stream.h err.h err.c vector.h vector.c safe_memory_operations.c safe_memory_operations.h)

add_executable (ToONP ToONP.c ${SOURCES})
add_executable (W W.c ${SOURCES} to_onp_algorithm.c to_onp_algorithm.h)
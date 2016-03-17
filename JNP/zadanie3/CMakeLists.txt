set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -g -O2")

set(SOURCES very_long_int.h very_long_int.cc)

add_executable (very_long_int ${SOURCES} main.cc)
add_executable (fb ${SOURCES} fb.cc)
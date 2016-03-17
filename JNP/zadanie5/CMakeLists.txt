set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -g -O2")

add_executable (main priorityqueue.hh main.cc)
add_executable (marek priorityqueue.hh marek.cc)
add_executable (piotr priorityqueue.hh piotrek.cc)
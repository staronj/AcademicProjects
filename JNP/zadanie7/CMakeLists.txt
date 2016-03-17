set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -g -O2")

add_executable (sample sample_bst_usage.cpp bst.h bst_imp.h)
add_executable (kuba_test1 kuba_test1.cpp bst.h bst_imp.h)
add_executable (piotr_test1 piotr_test1.cpp bst.h bst_imp.h)
add_executable (banaszkiewicz_test1 banaszkiewicz_test1.cpp bst.h bst_imp.h)
/**
 * Copyrights Jakub Staroñ 2015
 * numer indeksu 361025
 */

#include <unistd.h>
#include "string_builder.h"
#include "process_handle.h"
#include "to_onp_algorithm.h"

void W(ProcessHandle_pointer parent) {
  // read input and close reading descriptor
  String_pointer expression = ProcessHandle_readString(parent);
  String_pointer stack = ProcessHandle_readString(parent);
  String_pointer partial_result = ProcessHandle_readString(parent);
  ProcessHandle_closeRead(parent);

  // process the data
  MakeAlgorithmStep(&expression, &stack, &partial_result);

  // if expression is empty, this was the last algorithm step
  if (String_isEmpty(expression)) {
    // write final result to parent and close writing descriptor
    ProcessHandle_writeString(parent, partial_result);
    ProcessHandle_closeWrite(parent);
  }
  else {
    // create new children
    ProcessHandle_pointer handle = ProcessHandle_forkMe();
    if (ProcessHandle_amIChild(handle)) {
      // close last open descriptor from grandfather
      ProcessHandle_closeWrite(parent);
      W(handle);
    }
    else {
      // write data to child and close writing descriptor
      ProcessHandle_writeString(handle, expression);
      ProcessHandle_writeString(handle, stack);
      ProcessHandle_writeString(handle, partial_result);
      ProcessHandle_closeWrite(handle);

      // read response from child
      String_pointer result = ProcessHandle_readString(handle);
      ProcessHandle_closeRead(handle);

      // wait for child
      ProcessHandle_wait(handle);

      // write response to parent
      ProcessHandle_writeString(parent, result);
      ProcessHandle_closeWrite(parent);
      String_free(result);
    }
    ProcessHandle_free(handle);
  }

  String_free(expression);
  String_free(stack);
  String_free(partial_result);
}

int main(int argc, const char ** argv) {
  ProcessHandle_pointer parent = ProcessHandle_fromDescriptors(5, 4);
  W(parent);
  ProcessHandle_free(parent);

  close(0);
  close(1);
  close(2);

  return 0;
}

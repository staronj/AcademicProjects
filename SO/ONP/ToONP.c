/**
 * Copyrights Jakub Staroñ 2015
 * numer indeksu 361025
*/

#include <stdio.h>
#include <unistd.h>
#include "better_string.h"
#include "string_builder.h"
#include "process_handle.h"
#include "err.h"

bool IsNotEmpty(String_pointer string) {
  for (size_t i = 0 ; i < String_size(string) ; i++) {
    if (String_at(string, i) != ' ')
      return true;
  }
  return false;
}

void ToONP(const char * c_string) {
  // the initial conditions
  String_pointer expression = String_fromCString(c_string);
  String_pointer stack = String_fromCString("");
  String_pointer partial_result = String_fromCString("");

  // if expression is not empty, ie have at least one token
  if (IsNotEmpty(expression)) {
    ProcessHandle_pointer handle = ProcessHandle_forkMe();

    if (ProcessHandle_amIChild(handle)) {
      String_free(expression);
      String_free(stack);
      String_free(partial_result);

      execl("./W", "W", (char *)0);
      syserr("Error in execl\n");
    }
    else {
      ProcessHandle_writeString(handle, expression);
      ProcessHandle_writeString(handle, stack);
      ProcessHandle_writeString(handle, partial_result);
      ProcessHandle_closeWrite(handle);
      String_pointer result = ProcessHandle_readString(handle);
      ProcessHandle_closeRead(handle);
      puts(String_toCstring(result));
      String_free(result);

      ProcessHandle_wait(handle);
      ProcessHandle_free(handle);
    }
  }
  else {
    // result if expression was empty
    puts("");
  }

  String_free(expression);
  String_free(stack);
  String_free(partial_result);
}

int main(int argc, const char ** argv) {
  ToONP(argv[1]);

  close(0);
  close(1);
  close(2);

  return 0;
}
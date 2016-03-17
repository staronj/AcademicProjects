/**
 * Copyrights Jakub Staroñ 2015
 * numer indeksu 361025
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "process_handle.h"
#include "err.h"

struct ProcessHandle {
  pid_t child_pid;
  int read_pipe;
  int write_pipe;
};

static ProcessHandle_pointer ProcessHandle_allocate() {
  ProcessHandle_pointer result = malloc(sizeof(struct ProcessHandle));
  memset(result, 0, sizeof(struct ProcessHandle));
  return result;
}

ProcessHandle_pointer ProcessHandle_forkMe() {
  int pipe_first[2];
  int pipe_second[2];

  ProcessHandle_pointer result = ProcessHandle_allocate();

  if (pipe(pipe_first) == -1)
    syserr("Error in creating pipe\n");

  if (pipe(pipe_second) == -1)
    syserr("Error in creating pipe\n");

  result->child_pid = fork();
  switch (result->child_pid) {
    case -1:
      syserr("Error in fork\n");
      break;
    case 0:
      // child
      if (close (pipe_first[0]) == -1)
        syserr("Error in close (pipe_first[0])\n");

      if (close (pipe_second[1]) == -1)
        syserr("Error in close (pipe_second[1])\n");

      result->write_pipe = pipe_first[1];
      result->read_pipe = pipe_second[0];

      //printf("Child: write: %d, read: %d", result->write_pipe,  result->read_pipe);

      break;
    default:
      // parent
      if (close (pipe_first[1]) == -1)
        syserr("Error in close (pipe_first[1])\n");

      if (close (pipe_second[0]) == -1)
        syserr("Error in close (pipe_second[0])\n");

      result->read_pipe = pipe_first[0];
      result->write_pipe = pipe_second[1];
      break;
  }

  return result;
}

ProcessHandle_pointer ProcessHandle_fromDescriptors(int read_descriptor, int write_descriptor) {
  ProcessHandle_pointer result = ProcessHandle_allocate();
  result->child_pid = 0;
  result->write_pipe = write_descriptor;
  result->read_pipe = read_descriptor;
  return result;
}

void ProcessHandle_free(ProcessHandle_pointer this) {
  free(this);
}

bool ProcessHandle_amIChild(ProcessHandle_pointer this) {
  assert(this != NULL);
  return this->child_pid == 0;
}

void ProcessHandle_writeString(ProcessHandle_pointer this, String_pointer string) {
  assert(this != NULL);
  size_t size = String_size(string);
  if (write(this->write_pipe, &size, sizeof(size)) == -1)
    syserr("Error in write\n");

  if (write(this->write_pipe, String_toCstring(string), size) == -1)
    syserr("Error in write\n");
}

String_pointer ProcessHandle_readString(ProcessHandle_pointer this) {
  assert(this != NULL);
  size_t size = 0;

  if (read(this->read_pipe, &size, sizeof(size)) != sizeof(size))
    syserr("Error in reading size, pipe = %d\n", this->read_pipe);

  char * buffer = malloc(size);

  if (read(this->read_pipe, buffer, size) != size)
    syserr("Error in reading message, pipe = %d\n", this->read_pipe);

  String_pointer result = String_fromData(buffer, size);

  free(buffer);

  return result;
}

void ProcessHandle_closeRead(ProcessHandle_pointer this) {
  assert(this != NULL);
  close(this->read_pipe);
}

void ProcessHandle_closeWrite(ProcessHandle_pointer this) {
  assert(this != NULL);
  close(this->write_pipe);
}

void ProcessHandle_wait(ProcessHandle_pointer this) {
  assert(this != NULL);
  assert(this->child_pid > 0);
  if (waitpid(this->child_pid, 0, WUNTRACED) != this->child_pid)
    syserr("Error in waitpid\n");
}

/** Waitpid 1
 *
 * This program demonstrates the basic usage of the waitpid system call. A loop
 * creates a number of child processes by calling fork(). Another loop reaps
 * each of the children with waitpid and reports the exit status of each.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

// The number of child processes to create
#define NUM_CHILDREN 10

int main() {
  // Holds the exit status of a child
  int status;

  // The pid returned by fork()
  pid_t pid;

  for (int i = 0; i < NUM_CHILDREN; i++) {
    if ((pid = fork()) < 0) {
      fprintf(stderr, "error forking process: %s\n", strerror(errno));

      return 0;
    }

    if (pid == 0) {
      // Each child exits with a unique exit code which is set during waitpid.
      exit(100 + i);
    }
  }

  while ((pid = waitpid(-1, &status, 0)) > 0) {
    if (WIFEXITED(status)) {
      printf("child: %d exited normally with status: %d\n", pid,
             WEXITSTATUS(status));

      continue;
    }

    printf("child %d terminated abnormally\n", pid);
  }

  // ECHILD indicates there are no more child processes, which is desired.
  if (errno != ECHILD) {
    fprintf(stderr, "waitpid error %s\n", strerror(errno));

    return 1;
  }

  return 0;
}

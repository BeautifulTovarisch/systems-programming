/** Forking a Process
 *
 * This small program demonstrates the use of the fork() procedure, used to
 * create a process.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("Hello from: (pid:%d)\n", (int)getpid());

  // c_pid - This will be 0 when in the child process's execution context
  int c_pid = fork();

  if (c_pid < 0) {
    fprintf(stderr, "error forking process\n");

    return 1;
  }

  if (c_pid == 0) {
    printf("Hello from the child: (pid:%d)\n", (int)getpid());
  } else {
    printf("Hello from the parent: (pid:%d) of child: (pid:%d)\n",
           (int)getpid(), c_pid);
  }

  return 0;
}

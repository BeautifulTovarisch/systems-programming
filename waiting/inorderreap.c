/** Inorder Reap
 *
 * This program forks a number of child processes and then uses waitpid to reap
 * them in order.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 5

int main() {
  int status;

  // Use an array to store the pids of the forked processes.
  pid_t pid_list[NUM_CHILDREN];

  // Used to store the return of fork().
  pid_t cpid;

  int i;
  // Record the pids of the child processes
  for (i = 0; i < NUM_CHILDREN; i++) {
    if ((cpid = fork()) < 0) {
      fprintf(stderr, "error forking process: %s\n", strerror(errno));

      return 1;
    }

    pid_list[i++] = cpid;

    if (cpid == 0) {
      exit(100 + i);
    }
  }

  i = 0;
  // providing the specific pid as the first argument to waitpid will wait for
  // that specific child process to terminate. In this example, this has the
  // effect of waiting for each child in order of creation.
  while ((cpid = waitpid(pid_list[i++], &status, 0)) > 0) {
    if (WIFEXITED(status)) {
      printf("Child %d exited normally with status %d\n", cpid,
             WEXITSTATUS(status));

      continue;
    }

    fprintf(stderr, "Child %d exited abnormally\n", cpid);
  }

  if (errno != ECHILD) {
    fprintf(stderr, "waitpid error");

    return 1;
  }

  return 0;
}

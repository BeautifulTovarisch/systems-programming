/** Basic Signals
 *
 * This program demonstrates the most basic usage of passing signals between a
 * parent and child process. A child process is forked and then immediately
 * killed using kill()
 */

#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  pid_t pid;

  // Fork a child and use pause() to wait for a signal.
  if ((pid = fork()) < 0) {
    fprintf(stderr, "error forking process: %s\n", strerror(errno));

    return 1;
  }

  if (pid == 0) {
    // Pause only returns once the signal callback returns. In this example,
    // the process is killed immediately, making any code beneath this line
    // unreachable.
    pause();

    fprintf(stderr, "unreachable!");

    return 1;
  }

  if (kill(pid, SIGKILL) < 0) {
    fprintf(stderr, "error killing process (%d): %s\n", pid, strerror(errno));

    return 1;
  }

  return 0;
}

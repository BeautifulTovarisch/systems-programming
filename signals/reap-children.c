/** Reap All Children
 *
 * This program includes a flawed implementation of a SIGCHLD handler based on
 * the erroneous assumption that signals are queued  rather than processed at
 * most once. This leads to a concurrency bug in which child processes exit
 * while SIGCHLD is blocked.
 *
 * A second implementation fixes this issue by repeatedly invoking waitpid(),
 * therefore guaranteeing that child processes exiting while SIGCHLD is blocked
 * are reaped as well.
 */

#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>

// A flawed implementation of a SIGCHLD handler. The critical mistake here is
// the assumption that bad_handler will run every time SIGCHLD is sent. This
// leads to zombie child processes.
void bad_handler(int sig) {
  // save and restore errno
  int old_err = errno;
  if (waitpid(-1, NULL, 0) > 0) {
    char *msg = "reaped child\n";
    write(STDOUT_FILENO, msg, strlen(msg));
  }

  // Sleep here to increase the chances of ignored SIGCHLD messages. sleep is
  // not thread-safe in general.
  sleep(1);
  errno = old_err;

  return;
}

// The correct implementation of the handler. This version repeatedly reaps any
// child processes, including those that terminate while SIGCHLD is blocked.
void correct_handler(int sig) {
  int old = errno;

  while ((waitpid(-1, NULL, 0) > 0)) {
    char *msg = "reaped child\n";
    write(STDOUT_FILENO, msg, strlen(msg));
  }

  // This check is appropriate here, because we are blocking until all children
  // are reaped. Conspicuously absent in bad_handler, as all children are not
  // guaranteed to be reaped.
  if (errno != ECHILD) {
    char *msg = "waitpid error \n";
    write(STDERR_FILENO, msg, strlen(msg));
  }

  // sleep serves the same purpose as in the bad_handler.
  sleep(1);
  errno = old;
}

void sigint_handler(int sig) {
  char *msg = "SIGINT. Terminating\n";
  write(STDOUT_FILENO, msg, strlen(msg));

  exit(0);
}

int main(int argc, char *argv[]) {
  // Compare the behavior of installing bad_handler and correct_handler.
  if (signal(SIGCHLD, correct_handler) == SIG_ERR) {
    fprintf(stderr, "error registering bad_handler: %s\n", strerror(errno));

    return 1;
  }

  // Create 3 child processes
  pid_t pid;
  for (int i = 0; i < 3; i++) {
    if ((pid = fork()) < 0) {
      fprintf(stderr, "error forking child: %s\n", strerror(errno));

      return 1;
    }

    // Child says hello and then exits.
    if (pid == 0) {
      printf("creating child: %d\n", getpid());

      exit(0);
    }
  }

  // Allows the program to loop until SIGINT is sent.
  if (signal(SIGINT, sigint_handler) == SIG_ERR) {
    fprintf(stderr, "error registering sigint handler: %s\n", strerror(errno));

    return 1;
  }

  while (1)
    ;

  // Program should only exit if SIGINT is sent.
  return 1;
}

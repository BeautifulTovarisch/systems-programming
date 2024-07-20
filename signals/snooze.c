/** Snooze
 *
 * A basic program demonstrating signal handlers. This program accepts a single
 * argument determining the amount of time the process sleeps and prints the
 * elapsed time unless interrupted by SIGINT.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sigint_handler(int sig) {
  printf("Received (%s). Terminating.\n", strsignal(sig));

  exit(0);
}

int main(int argc, char *argv[]) {
  char *endptr;

  if (argc < 2) {
    fprintf(stderr, "usage: %s <integer>\n", argv[0]);

    return 1;
  }

  if (signal(SIGINT, sigint_handler) == SIG_ERR) {
    fprintf(stderr, "error registering SIGINT handler: %d\n", strerror(errno));

    return 1;
  }

  // Read in the time to sleep.
  long duration = strtol(argv[1], &endptr, 10);

  unsigned int rem = sleep(duration);

  printf("Slept for %u seconds\n", (duration - rem));

  return 0;
}

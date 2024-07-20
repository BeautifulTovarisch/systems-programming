/** Signal Alarm
 *
 * This program uses the alarm() system call to demonstrate a process sending a
 * signal to itself. A signal handler (callback) catches SIGALRM and outputs a
 * message.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Create a signal handler for the SIGALRM signal.
void alarm_handler(int signal) {
  printf("SIGALM caught! Terminating");

  return;
}

// Register a signal handler for alarm and immediately invoke alarm
int main(int argc, char *argv[]) {
  if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
    fprintf(stderr, "error registering signal handler: %s\n", strerror(errno));

    return 1;
  }

  // Send SIGALRM after n seconds (merely a suggestion to kernel)
  alarm(2);

  pause();

  return 0;
}

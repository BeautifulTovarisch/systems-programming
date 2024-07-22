/** Signal Suspend
 *
 * This basic program forks a child process and then waits for the receipt of
 * SIGCHLD before proceeding. The main loop simulates the lifecycle of a shell:
 *
 *  1. Accept user command
 *  2. Fork child process and execute command
 *  3. Wait for SIGCHLD to be received before processing next command
 *
 * Step 3 uses sigsuspend() instead of the more wasteful spin loop:
 *
 *  while(!pid)
 *    sleep(1); // or some equivalent
 *
 * This has the effect of putting the main shell loop to "sleep" until SIGCHLD
 * is received.
 *
 * A great deal of error-handling is omitted for brevity.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>

volatile sig_atomic_t pid;

void sigchld_handler(int sig) {
  int old = errno;

  pid = waitpid(-1, NULL, 0);

  errno = old;
}

int main(int argc, char *argv[]) {
  sigset_t mask, prev_mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGCHLD);

  signal(SIGCHLD, sigchld_handler);

  sigprocmask(SIG_BLOCK, &mask, &prev_mask);
  // Child process simply exists immediately, triggering SIGCHLD.
  if (fork() == 0) {
    return 0;
  }

  // Initialize pid to 0 here to serve as a sentinel. The call to waitpid will
  // set this to a nonzero pid once the child process is reaped.
  pid = 0;
  while (!pid) {
    sigsuspend(&prev_mask);
  }

  sigprocmask(SIG_SETMASK, &prev_mask, NULL);

  printf("SIGCHLD received, continuing execution of parent\n");

  return 0;
}

/** Signal Blocking
 *
 * This program protects a small critical path from SIGUSR1 signals via explicit
 * blocking with sigprocmask. This is conceptually similar to using a mutex.
 *
 * The following are the high-level steps involved:
 *
 *  1. Initialize a sigset_t with either sigemptyset() or sigfillset()
 *    a. Additionally declare a sigset_t to store the previous mask state.
 *    b. Register the desired signal with sigaddset (if sigemptyset was used)
 *
 *  2. Use sigprocmask() with SIG_BLOCK
 *
 *  3. Perform the work in the critical path
 *
 *  4. Use sigprocmask() with SIG_SETMASK and the previous mask set, restoring
 *    the previous state of the blocked signal mask.
 *    a. SIG_UNBLOCK removes the signals in a given set from the blocked mask.
 *      TODO: Determine when one is more appropriate than the other.
 *
 * The control flow of the program is also intentionally complex in order to
 * demonstrate synchronizing behavior between a parent and child process.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>

// Global flags/variables must be declared as volatile to force the compiler to
// read from memory rather than cache the data (say in a register). This arises
// when a variable is mutated within a handler, but appears to be unmodified in
// main() from the perspective of the compiler.
volatile long counter = 2;

// SIGUSR1 handler used to demonstrate blocking the SIGUSR1 signal during some
// critical path in main().
//
// This handler blocks all signals and updates the global counter. Crucially,
// this handler is used by the parent process to terminate the child
void usr1_handler(int sig) {
  int old_err = errno;

  sigset_t mask, prev_mask;

  // Block all signals. Error handling is omitted as examples appear in main().
  sigfillset(&mask);
  sigprocmask(SIG_BLOCK, &mask, &prev_mask);

  char *msg = "Hello from sigint_handler\n";
  write(STDOUT_FILENO, msg, strlen(msg));

  // This counter is decremented ONLY during the CHILD process!
  --counter;

  sigprocmask(SIG_SETMASK, &prev_mask, NULL);

  errno = old_err;

  // Normal exit is not signal safe. See man 3 exit.
  _exit(0);
}

int main(int argc, char *argv[]) {
  // Declare current and previous signal masks
  sigset_t mask, prev_mask;

  printf("counter: %ld\n", counter);
  fflush(stdout);

  // Registering the signal handler after printing ensures no data race until
  // after this point.
  if (signal(SIGUSR1, usr1_handler) == SIG_ERR) {
    fprintf(stderr, "error isntalling alarm handler: %s\n", strerror(errno));

    return 1;
  }

  // Fork a child process that simply loops until terminated by the parent;
  pid_t pid;
  if ((pid = fork()) == 0) {
    while (1)
      ;
  }

  // Immediately send SIGUSR1. This will trigger the corresponding handler and
  // terminate the child process while we wait with waitpid.
  kill(pid, SIGUSR1);
  if (waitpid(-1, NULL, 0) < 0) {
    fprintf(stderr, "waitpid error: %s\n", strerror(errno));

    return 1;
  }

  // Block all signals
  sigfillset(&mask);

  // Everything beneath this line until the second invocation of sigprocmask is
  // can't be interrupted by SIGINT.
  if (sigprocmask(SIG_BLOCK, &mask, &prev_mask) < 0) {
    fprintf(stderr, "error blocking signal: %s\n", strerror(errno));

    return 1;
  }

  printf("counter: %ld\n", --counter);

  // Unblock the critical path.
  if (sigprocmask(SIG_SETMASK, &prev_mask, NULL) < 0) {
    fprintf(stderr, "error unblocking signal: %s\n", strerror(errno));

    return 1;
  }

  return 0;
}

/** Signal Jumps
 *
 * This small program demonstrates a trivial jump from a SIGINT handler. For a
 * bit of variety, the main() procedure also uses sigsuspend() to explicitly
 * wait for SIGINT.
 *
 * The main body of the program shows a way to catch SIGINT and jump back to
 * previously executed code. A sentinel value is set by the handler to avoid an
 * infinite loop.
 */

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>

// Signal analogue to jmp_buf.
sigjmp_buf buf;

// Flag used in the guard of the main program loop.
volatile sig_atomic_t exit_flag;

// This signal handler contains only a call to siglongjmp, which immediately
// returns control back to a location in main.
void sigint_handler(int sig) {
  exit_flag++;
  siglongjmp(buf, 1);
}

int main(int argc, char *argv[]) {
  sigset_t mask, prev;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  signal(SIGINT, sigint_handler);

  // Technically not needed, since exit_flag could just be initialized inline,
  // but for variety SIGINT is blocked here to demonstrate synchronization.
  sigprocmask(SIG_BLOCK, &mask, &prev);
  exit_flag = 0;
  sigprocmask(SIG_SETMASK, &prev, NULL);

  // The SIGINT handler jumps here.
  if (sigsetjmp(buf, 1)) {
    printf("exit flag set to %d\n", exit_flag);
  }

  // Pause the thread until SIGINT is handled and exit_flag is incremented to 2.
  while (exit_flag < 2)
    sigsuspend(&prev);

  return 0;
}

/** Signal Suspend
 *
 * This program demonstrates how to use the sigsuspend() procedure in order to
 * cause a process to wait for a signal handler to run before proceeding. This
 * can be accomplished with a spin loop:
 *
 *  while (<some condition>);
 *
 * but this is a considerable waste of CPU resources.
 *
 * The main function registers a SIGCHLD handler and then blocks SIGCHLD while
 * it does some work.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>

volatile sig_atomic_t pid;

void addjob(pid_t pid) { printf("Adding job(%d)\n", pid); };

void deljob(pid_t pid) {
  char *msg = "deleting job\n";
  write(STDOUT_FILENO, msg, strlen(msg));
};

void sigchld_handler(int sig) {
  int old = errno;

  sigset_t mask_all, prev_mask;
  sigfillset(&mask_all);

  pid_t pid;
  // Reap children. The body of the loop blocks all signals while deleting the
  // child pids from the jobs list.
  while ((pid = waitpid(-1, NULL, 0)) > 0) {
    sigprocmask(SIG_BLOCK, &mask_all, &prev_mask);
    deljob(pid);
    sigprocmask(SIG_SETMASK, &prev_mask, NULL);
  }

  if (errno != ECHILD) {
    char *msg = "waitpid error";
    write(STDOUT_FILENO, msg, strlen(msg));
  }

  errno = old;
}

// The core functionality of the program is to simulate forking a process which
// runs some command, similar to a shell. In a real shell, jobs are kept track
// of in some kind of data structure.
int main(int argc, char *argv[]) {
  sigset_t mask_all, prev_mask;
  sigfillset(&mask_all);
  signal(SIGCHLD, sigchld_handler);

  pid_t pid;
  // Loop forever to simulate a shell program.
  while (1) {
    // Blocking before forking the child process ensures SIGCHLD is not caught
    // before addjob() is called from the parent.
    sigprocmask(SIG_BLOCK, &mask_all, &prev_mask);
    if ((pid = fork()) == 0) {
      // The child process exec()'s the desired command.
      execve("/bin/date", argv, NULL);

      // This is only reachable if execve fails.
      sigprocmask(SIG_SETMASK, &prev_mask, NULL);
    }

    // Access to the jobs list is synchronized, ensuring SIGCHLD is not handled
    // before the job is added.
    addjob(pid);
    sigprocmask(SIG_SETMASK, &prev_mask, NULL);
  }

  // Unreachable.
  return 0;
}

/** Pipe
 *
 * This program forks two children and pipes the STDOUT of one into the STDIN of
 * the other. This is used as an example of how to use the pipe() system call.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFSIZE (1 << 6)

pid_t Fork() {
  pid_t pid;

  if ((pid = fork()) < 0) {
    fprintf(stderr, "error forking child: %s\n", strerror(errno));

    exit(EXIT_FAILURE);
  }

  return pid;
}

int main(int argc, char *argv[argc]) {
  // Create the pipe.
  int pipefds[2];
  if (pipe(pipefds) < 0) {
    fprintf(stderr, "error creating pipe: %s\n", strerror(errno));

    return EXIT_FAILURE;
  }

  pid_t p1, p2;

  // Setup the 'writer' child. This child closes its copy of the read end of
  // the pipe and redirects its STDOUT to the write end.
  if ((p1 = Fork()) == 0) {
    close(pipefds[0]);
    dup2(pipefds[1], STDOUT_FILENO);

    printf("hello from child 1!");

    return EXIT_SUCCESS;
  } else {
    // Now the parent process can fork the second child, which can perform the
    // inverse setup to the first.

    // This is the reader child. Redirect the reader stream to STDIN
    if ((p2 = Fork()) == 0) {
      close(pipefds[1]);
      dup2(pipefds[0], STDIN_FILENO);

      char buf[BUFSIZE];
      read(STDIN_FILENO, buf, BUFSIZE);

      printf("message received: %s\n", buf);

      return EXIT_SUCCESS;
    }
  }

  waitpid(p2, (void *)0, 0);

  return EXIT_SUCCESS;
}

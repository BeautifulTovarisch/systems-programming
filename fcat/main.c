/** File Cat (working title)
 *
 * This program demonstrates the subtle behavior of file descriptors. The main
 * routine forks a child process which closes its copy of STDIN and invokes the
 * cat program on 'input.txt'. Because STDIN is closed, the subsequent call to
 * open reuses file descriptor 0 as it is the lowest free descriptor.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>

int main(int argc, char *argv[argc]) {
  (void)argv;

  pid_t cpid = fork();
  if (cpid < 0) {
    fprintf(stderr, "error forking child: %s\n", strerror(errno));

    return EXIT_FAILURE;
  }

  if (cpid == 0) {
    // Ensure CAT reads from fd=0 once STDIN is closed.
    char *args[2] = {"/usr/bin/cat", STDIN_FILENO};

    close(STDIN_FILENO);
    open("./input.txt", O_RDONLY);

    // Only returns on error
    execv("/usr/bin/cat", args);
    fprintf(stderr, "exec error: %s\n", strerror(errno));

    return EXIT_FAILURE;
  }

  if ((waitpid(cpid, (int *)0, 0)) < 0) {
    fprintf(stderr, "waitpid error: %s\n", strerror(errno));

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

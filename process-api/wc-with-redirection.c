/** Word Count with Redirection
 *
 * This program performs a word count of it source code and ouputs the result
 * to a file using redirection.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int cpid = fork();

  if (cpid < 0) {
    fprintf(stderr, "error forking process\n");

    return 1;
  }

  if (cpid == 0) {
    // Redirect STDOUT of wc program to wc.txt
    // NOTE: I'm not sure this is the most clear way to do this...
    close(STDOUT_FILENO);
    open("./wc.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

    char *args[3] = {"wc", "wc-with-redirection.c", NULL};

    execvp(args[0], args);
  } else {
    wait(NULL);
  }

  return 0;
}

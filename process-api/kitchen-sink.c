/** Kitchen Sink
 *
 * This program demonstrates the interplay between fork(), wait(), and exec()
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int cpid = fork();

  if (cpid < 0) {
    fprintf(stderr, "error forking process\n");

    return 1;
  }

  // The child will run word count on this source file using exec()
  if (cpid == 0) {
    printf("Initiating wordcount (pid:%d)\n", (int)getpid());
    char *args[3] = {"wc", "kitchen-sink.c", NULL};

    execvp(args[0], args);
    fprintf(stderr, "should be unreachable");
  } else {
    printf("waiting on (child:%d) to finish\n", wait(NULL));
  }

  printf("mission accomplished\n");

  return 0;
}

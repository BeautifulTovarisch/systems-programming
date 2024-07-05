/* For with Wait
 *
 * This program uses the wait() system call in order to cause it to behave in a
 * deterministic manner.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int p = getpid();

  int cpid = fork();

  if (cpid < 0) {
    fprintf(stderr, "Error forking process\n");

    return 1;
  }

  if (cpid == 0) {
    printf("I am the child: (pid:%d)\n", (int)getpid());
  } else {
    // Wait for the child to finish its task first.
    int ret = wait(NULL);
    printf("I, (pid:%d), am the parent of (pid:%d), also known as (wait:%d)\n",
           p, cpid, ret);
  }

  return 0;
}

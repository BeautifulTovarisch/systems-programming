/** Hello Threads
 *
 * This program introduces the basic usage of pthreads. Its sole functionality
 * is to use a thread handler to output "hello, world" to STDOUT.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Don't use NULL!
#define NIL ((void *)0)

void *threadfn(void *vargp) {
  // Unused
  (void)vargp;

  printf("hello, world!\n");

  return NIL;
}

int main(int argc, char *argv[argc]) {
  // Unused
  (void)argv;

  pthread_t thread_id;

  // Minimal invocation to create a thread.
  int rc = pthread_create(&thread_id, NIL, threadfn, NIL);
  if (rc != 0) {
    fprintf(stderr, "error creating thread: %s\n", strerror(rc));

    return EXIT_FAILURE;
  }

  // Wait for the thread to finish its execution.
  if ((rc = pthread_join(thread_id, NIL))) {
    fprintf(stderr, "pthread_join error: %s\n", strerror(rc));

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

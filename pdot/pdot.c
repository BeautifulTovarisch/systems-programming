/* Dot Product
 *
 * This program computes the dot product of two randomized vectors whose length
 * is given by a command-line argument. This algorithm avoids synchronization by
 * partitioning the input vectors and computing a portion of the dot product at
 * time.
 *
 * USAGE:
 *  ./pdot <length>
 */

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Size of the partition handled by a given thread.
#define PSIZE (1 << 14)

// pdot_arg is the argument to the pdot thread handler. It contains the required
// information for the thread handler to compute its portion of the dot product
// and store its result to be aggregated later.
typedef struct {
  long *u;  // Pointer to vector [u]
  long *v;  // Pointer to vector [v]
  long res; // Accumulated result
} pdot_arg;

// This can be used as-is by providing n=PSIZE and offsetting each vector by the
// appropriate number of elements.
long dot(size_t n, long u[n], long v[n]) {
  long sum = 0;

  for (size_t i = 0; i < n; i++)
    sum += u[i] * v[i];

  return sum;
}

// pdot accepts a struct with the appropriate section of each vector it is meant
// to handle. The result of the computation is stored in the input struct for
// later aggregation.
void *pdot(void *vargp) {
  pdot_arg *arg = (pdot_arg *)vargp;

  long res = dot(PSIZE, arg->u, arg->v);

  arg->res = res;

  return (void *)0;
}

// sequential implementation. This is valuable to compare against the parallel
// implementation, pdot.
long sdot(size_t n, long u[n], long v[n]) { return dot(n, u, v); }

// read the provided length. This function will exit if an invalid argument is
// given.
size_t readlen(char *length) {
  char *end;
  long n = strtol(length, &end, 10);

  if (end == length) {
    fprintf(stderr, "invalid argument %s: %s\n", length, strerror(errno));

    exit(EXIT_FAILURE);
  }

  if (n <= 0) {
    fprintf(stderr, "length must be greater than 0, got: %ld\n", n);

    exit(EXIT_FAILURE);
  }

  return (size_t)n;
}

// Insert random integers between 1 and 10 into the provided vector. I don't use
// a seed for this program to make debugging easier, but one can be provided if
// the programmer would like an extra challenge. See srand().
void randomize(size_t n, long vec[n]) {
  for (size_t i = 0; i < n; i++) {
    vec[i] = (rand() % 10) + 1;
  }
}

void pthread_error(int code) {
  if (code) {
    fprintf(stderr, "error creating thread: %s\n", strerror(code));

    exit(EXIT_FAILURE);
  }
}

void print_array(size_t n, long vec[n]) {
  printf("[ ");
  for (size_t i = 0; i < n; i++)
    printf(" %ld ", vec[i]);

  printf("]\n");
}

long *create_vector(size_t len) {
  long *vec = calloc(len, sizeof(long));

  randomize(len, vec);

  return vec;
}

long compute(size_t length, long *u, long *v) {
  // nthreads only handles elements that can be chunked in this way. The rest
  // are handled in the main thread. This latch batch is O(PSIZE-1).
  size_t nthreads = length / PSIZE;

  // Allocate the correct number of structs to provide to thread functions and
  // bookeeping arrays to manage variable amount of threads.
  pdot_arg *args = calloc(nthreads, sizeof(pdot_arg));
  pthread_t *threads = calloc(nthreads, sizeof(pthread_t));

  for (size_t i = 0; i < nthreads; i++) {
    // Offset the vectors by the appropriate number of elements
    size_t offset = i * PSIZE;

    args[i].u = (u + offset);
    args[i].v = (v + offset);
    args[i].res = 0;

    pthread_error(pthread_create(&threads[i], (void *)0, pdot, &args[i]));
  }

  // Wait for all threads to finish.
  for (size_t i = 0; i < nthreads; i++) {
    pthread_error(pthread_join(threads[i], (void *)0));
  }

  long sum = 0;
  for (size_t i = 0; i < nthreads; i++) {
    sum += args[i].res;
  }

  // This is the index of the first unprocessed element. If length is a multiple
  // of PSIZE, [rest] will equal [length], and the loop guard below will fail.
  //
  // This is either somewhat clever or profoundly stupid.
  size_t rest = length - (length % PSIZE);

  // Process remaining elements.
  for (size_t i = rest; i < length; i++) {
    sum += u[i] * v[i];
  }

  free(args);
  free(threads);

  return sum;
}

int main(int argc, char *argv[argc]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <length>\n", argv[0]);

    return EXIT_FAILURE;
  }

  size_t length = readlen(argv[1]);

  long *u = create_vector(length);
  long *v = create_vector(length);

  if (!(u && v)) {
    fprintf(stderr, "failed to allocate vectors error: %s\n", strerror(errno));

    return EXIT_FAILURE;
  }

  long dot = compute(length, u, v);

  printf("dot = %ld\n", dot);

  free(u);
  free(v);

  return EXIT_SUCCESS;
}

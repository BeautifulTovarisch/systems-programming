/** Prefix Sum
 *
 * This program accepts a list of integers and computes its "prefix sum", which
 * is defined by the recurrence:
 *
 *  p(0) = a[0]
 *  p(i) = p(i-1) + a[i]
 *
 * The first implementation performs the computation in the obvious way, and
 * the second uses a technique called "loop unrolling" in order to compute an
 * additional element per iteration.
 *
 * NOTE: Loop unrolling by hand is questionable, as many modern compilers will
 * perform this without needing special treatment from the programmer.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Output the vector in the form: [ v1, v2, .... vn ]
void print_vector(size_t size, long vector[size]) {
  printf("[");
  for (size_t i = 0; i < size; i++) {
    printf(" %ld ", vector[i]);
  }
  printf("]\n");
}

// Read a [size] space separated integers from [argv] into [vector].
void readargs(size_t size, char *argv[size + 1], long vector[size]) {
  char *end;
  for (size_t i = 1; i <= size; i++) {
    vector[i - 1] = strtol(argv[i], &end, 10);
  }
}

// psum1 computes the prefix sum of [vector], storing the output in [out]. This
// implementation is a straightforward adaptation of the recurrence.
void psum1(size_t n, long vector[n], long out[n]) {
  out[0] = vector[0];

  for (size_t i = 1; i < n; i++) {
    out[i] = out[i - 1] + vector[i];
  }
}

// psum1 computes the prefix sum of [vector] as in [psum1], unrolling the inner
// loop in order to compute two values per iteration.
void psum2(size_t size, long vector[size], long out[size]) {
  size_t i = 1;
  out[0] = vector[0];

  // Compute out[i] and out[i+1] each iteration.
  for (; i < size - 1; i += 2) {
    long prefix = out[i - 1] + vector[i];
    out[i] = prefix;
    out[i + 1] = prefix + vector[i + 1];
  }

  // Any remaining elements
  if (i < size) {
    out[i] = out[i - 1] + vector[i];
  }
}

int main(int argc, char *argv[argc]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <integers>\n", argv[0]);

    return EXIT_FAILURE;
  }

  size_t size = argc - 1;
  long *vector = calloc(size, sizeof(long));
  long *out1 = calloc(size, sizeof(long));
  long *out2 = calloc(size, sizeof(long));

  // TODO: Think about how error handling should occur.
  readargs(size, argv, vector);

  psum1(size, vector, out1);
  psum2(size, vector, out2);

  print_vector(size, vector);
  print_vector(size, out1);
  print_vector(size, out2);

  free(vector);
  free(out1);
  free(out2);
}

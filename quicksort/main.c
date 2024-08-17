/** Quicksort
 *
 * This program accepts a list of arguments on the command line and sorts them
 * in nondecreasing order. Non-randomized quicksort is used to sort the input
 * arguments in-place.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(char *A[], size_t i, size_t j) {
  char *tmp = A[i];
  A[i] = A[j];
  A[j] = tmp;
}

// For this implementation, A[lo] is always chosen as the pivot.
size_t partition(char *A[], size_t lo, size_t hi) {
  char *pivot = A[lo];

  // i advances across the 'frontier' of the unsorted portion of [A].
  size_t i = lo + 1;

  for (size_t j = i; j < hi; j++) {
    if (strcmp(A[j], pivot) < 0) {
      swap(A, i, j);
      i++;
    }
  }

  swap(A, i - 1, lo);

  return i - 1;
}

void quicksort(char *A[], size_t lo, size_t hi) {
  if (lo >= hi) {
    return;
  }

  size_t p_idx = partition(A, lo, hi);

  quicksort(A, lo, p_idx);
  quicksort(A, p_idx + 1, hi);

  return;
}

int main(int argc, char *argv[argc]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <strings>\n", argv[0]);

    return EXIT_FAILURE;
  }

  char *A[argc];

  for (size_t i = 1; i < argc; i++) {
    A[i - 1] = argv[i];
  }

  // Hi here is the length of the list, not the last index.
  quicksort(A, 0, argc - 1);

  printf("A = [");
  for (size_t i = 0; i < argc - 1; i++) {
    printf(" %s ", A[i]);
  }

  printf("]\n");
}

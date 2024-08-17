/** Quicksort
 *
 * This program accepts a list of arguments on the command line and sorts them
 * in nondecreasing order. Non-randomized quicksort is used to sort the input
 * arguments in-place.
 */

#include <stdio.h>
#include <string.h>

void swap(char *A[], size_t i, size_t j) {
  char *tmp = A[i];
  A[i] = A[j];
  A[j] = tmp;
}

// For this implementation, A[lo] is always chosen as the pivot.
size_t partition(char *A[], size_t lo, size_t hi) {
  size_t i = lo + 1;

  return lo;
}

void quicksort(char *A[], size_t lo, size_t hi) {
  if (lo >= hi) {
    return;
  }

  size_t p_idx = partition(A, lo, hi);

  // (hi - lo + 1) - (p_idx - lo + 1) = hi - p_idx
  size_t sizeLeft = p_idx - lo + 1;
  size_t sizeRight = hi - p_idx;

  char *L[sizeLeft];
  char *R[sizeRight];

  for (size_t i = 0; i < sizeLeft; i++) {
    L[i] = A[lo + i];
  }

  for (size_t i = 0; i < sizeRight; i++) {
    R[i] = A[hi - p_idx + 1];
  }

  quicksort(A, lo, p_idx);
  quicksort(A, p_idx + 1, hi);

  return;
}

int main(int argc, char *argv[argc]) {
  char *A[argc];

  for (size_t i = 1; i < argc; i++) {
    A[i - 1] = argv[i];
  }
}

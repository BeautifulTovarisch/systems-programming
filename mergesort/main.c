/** Merge Sort
 *
 * This program is a recursive implementation of Merge Sort done as a simple
 * exercise. The sort is performed in-place on the input array.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void print_array(size_t n, char *A[n]) {
  printf("[ ");
  for (size_t i = 0; i < n; i++) {
    printf("%s ", A[i]);
  }

  printf("]\n");
}

void merge(char *A[], size_t lo, size_t hi) {
  // The total number of elements is (hi-lo+1). Therefore, if the left subarray
  // has (mid-lo+1) There are (hi-lo+1) - (mid-lo+1) elements remaining:
  //
  // (hi-lo+1) - (mid-lo+1) = hi - mid
  size_t mid = lo + (hi - lo) / 2;
  size_t sizeLeft = mid - lo + 1;
  size_t sizeRight = hi - mid;

  char *L[sizeLeft];
  char *R[sizeRight];

  for (size_t i = 0; i < sizeLeft; i++) {
    L[i] = A[lo + i];
  }

  for (size_t i = 0; i < sizeRight; i++) {
    R[i] = A[mid + i + 1];
  }

  size_t i = 0, j = 0, k = lo;
  // Sort the subarray A[lo:hi] by copying in the lesser value from [L] and [R]
  while (i < sizeLeft && j < sizeRight) {
    if (strcmp(L[i], R[j]) < 0) {
      A[k++] = L[i++];
    } else {
      A[k++] = R[j++];
    }
  }

  while (i < sizeLeft) {
    A[k++] = L[i++];
  }

  while (j < sizeRight) {
    A[k++] = R[j++];
  }
}

void mergesort(char *list[], size_t lo, size_t hi) {
  if (lo >= hi) {
    return;
  }

  // This is to prevent overflow
  size_t mid = lo + (hi - lo) / 2;

  mergesort(list, lo, mid);
  mergesort(list, mid + 1, hi);

  merge(list, lo, hi);
}

bool is_sorted(size_t n, char *list[n]) {
  if (n <= 1) {
    return true;
  }

  for (size_t i = 1; i < n; i++) {
    if (strcmp(list[i - 1], list[i]) > 0) {
      return false;
    }
  }

  return true;
}

int main(int argc, char *argv[argc]) {
  char *A[argc];

  for (size_t i = 1; i < argc; i++) {
    A[i - 1] = argv[i];
  }

  mergesort(A, 0, argc - 2);

  printf("mergesort(A) = ");
  print_array(argc - 1, A);
}

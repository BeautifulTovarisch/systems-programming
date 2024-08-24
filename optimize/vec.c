#include "vec.h"

#include <stdlib.h>

vector *new_vec(long len) {
  vector *v = malloc(sizeof(vector));
  // Failed to allocate vector
  if (!v) {
    return (void *)0;
  }

  data_t *data = calloc(len, sizeof(data_t));
  // Unable to allocate data elements, clean up vector and return a nil pointer
  if (!data) {
    free(v);

    return (void *)0;
  }

  v->len = len;
  v->data = data;

  return v;
}

int get_vec_element(vector *vec, size_t idx, data_t *data) {
  // Removing this bounds-checking could be an optimization, especially since
  // this requires the client to introduce an additional conditional statement
  // to check the return value of this function.
  if (idx < 0 || idx >= vec->len) {
    return 0;
  }

  *data = vec->data[idx];

  return 1;
}

void fold_vec1(vector *vec, data_t *dest) {
  *dest = IDENT;

  // Accessing [len] from vec repeatedly reads from memory.
  for (size_t i = 0; i < vec->len; i++) {
    data_t d;
    get_vec_element(vec, i, &d);

    // Accumulate the value in [dest] directly. This results in unnecessary
    // writes to main memory.
    *dest = *dest OP d;
  }
}

void fold_vec2(vector *vec, data_t *dest) {
  // This optimization is an example of "code motion", wherein a computation
  // is moved outside of a loop to be reused rather than needlessly recomputed.
  size_t len = vec->len;

  *dest = IDENT;
  for (size_t i = 0; i < len; i++) {
    data_t d;
    get_vec_element(vec, i, &d);

    *dest = *dest OP d;
  }
}

void fold_vec3(vector *vec, data_t *dest) {
  // This eliminates the repeated call to get_vec_element, but usually does not
  // result in significant (or any) performance improvements. This does let us
  // make additional performance improvements by el
  data_t *data = vec->data;
  size_t len = vec->len;

  *dest = IDENT;
  for (size_t i = 0; i < len; i++) {
    // This remains the bottleneck in the program. The repeated memory accesses
    // here will typically eclipse many other kinds of inefficiencies.
    *dest = *dest OP data[i];
  }
}

void fold_vec4(vector *vec, data_t *dest) {
  data_t *data = vec->data;
  size_t len = vec->len;

  // This implementation introduces a temporary variable allocated either on
  // the stack or in an register. Updating these memory devices is orders of
  // magnitude faster than repeatedly updating main memory.
  data_t acc = IDENT;
  for (size_t i = 0; i < len; i++) {
    acc = acc OP data[i];
  }

  *dest = acc;
}

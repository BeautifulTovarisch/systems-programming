/** Vector
 *
 * This module defines a vector type to be used in an example program used to
 * demonstrate performance optimization techniques
 */

#ifndef VECTOR_H
#define VECTOR_H

// OP defines the operation to be used when folding a vector into a scalar.
#define OP +
// IDENT defines the scalar identity value corresponding to OP
#define IDENT 0

#include <stddef.h>

// data_t is the type of component in a vector.
typedef long data_t;

// vector is holds a number of abstract data elements.
typedef struct {
  size_t len;   // The number of components in the vector
  data_t *data; // A pointer to the first component in the vector.
} vector;

// new_vec returns a pointer to a vector with [len] elements. Elements take on
// the 0 value of [data_t].
vector *new_vec(long len);

// get_vec_element attempts to retrieve in [vec] at index [idx], storing its
// value in [dest]. The function returns 0 if [idx] is out of bounds and an
// arbitrary integer otherwise.
//
// The valid range of vector indices is from [0, len-1], as with an array.
//
// Example:
//
//  if (!get_vec_element(vector, 0, &data)) {
//    ... handle error ...
//  }
int get_vec_element(vector *vec, size_t idx, data_t *data);

// fold_vec1 combines the elements of [vec] according to [OP] and [IDENT],
// storing the value in [dest].
//
// This implementation is the most straightforward (and least optimized).
void fold(vector *vec, data_t *dest);

#endif

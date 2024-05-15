/**
 * Show Bytes
 *
 * This small program demonstrates the use of casting to circumvent C's types.
 * Each function accepts a different type and prints out the byte sequences of
 * the underlying data.
 */

#include <stdio.h>

// byte_pointer refers to an individual byte of a data object.
typedef unsigned char *byte_pointer;

// show_bytes iterates over the byte sequence beginning at [start] and outputs
// the hexadecimal representation of each byte in the given range.
void show_bytes(byte_pointer start, size_t len) {
  // Since each data type is cast to a *byte_pointer, we can index into [start]
  // just as we would any other array.
  for (int i = 0; i < len; i++) {
    printf(" %.2x", start[i]);
  }

  printf("\n");
}

// show_int displays the byte sequence of integer [x]
void show_int(int x) { show_bytes((byte_pointer)&x, sizeof(int)); }

// show_float displays the byte sequence of float [x]
void show_float(float x) { show_bytes((byte_pointer)&x, sizeof(float)); }

// show_pointer prints the byte sequence of an arbitrary pointer
void show_pointer(void *p) { show_bytes((byte_pointer)&p, sizeof(void *)); }

int main() {
  int x = 10;
  int *p = &x;
  float f = (float)x;

  show_int(x);
  show_float(f);
  show_pointer(p);
}

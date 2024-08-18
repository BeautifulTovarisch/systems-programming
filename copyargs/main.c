/** Copy Arguments
 *
 * This small program demonstrates some subtleties in dealing with strings vs
 * arrays and also provides a way to copy the input arguments of the program to
 * an array using [memcpy], an task I routinely forget how to do safely.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This program copies the first argument (the name of the program) into a
// variable-length array called name.
//
// In general, library functions that require array arguments have the "mem"
// prefix and those that operate on strings begin with "str". If it is not known
// whether the string is properly null-terminated, the "mem" variants must be
// used as using "str*" on non-strings is undefined.
int main(int argc, char *argv[argc + 1]) {
  // First we measure the length of the argument.
  size_t const len = strlen(argv[0]);

  // We need to allocate an extra byte for null-termination
  char name[len + 1];

  // Copy [len] bytes into [name] from argv[0].
  memcpy(name, argv[0], len);

  // Explicitly terminate the string.
  name[len] = '\0';

  if (!strcmp(name, argv[0])) {
    printf("The program name is %s\n", name);

    return EXIT_SUCCESS;
  }

  printf("error copying program name. Expected %s. Got %s\n", argv[0], name);

  return EXIT_FAILURE;
}

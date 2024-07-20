/** (Safe) Signal I/O
 *
 * This module implements basic I/O procedures that are safe for use in signal
 * handlers. These are suitable replacements for the non-safe functions found
 * in the <stdio.h> library.
 */

#include <stddef.h>
#include <unistd.h>

// TODO: Verify if this results in the same ASM as using char s[] and indexing.
static size_t sio_strlen(char *s) {
  size_t len = 0;

  while (*(s++)) {
    len++;
  }

  return len;
};

// sio_puts outputs the contents of [msg] to STDOUT via direct call to write().
// The function returns the numbers of bytes written or -1 on errors.
ssize_t sio_puts(char *msg);

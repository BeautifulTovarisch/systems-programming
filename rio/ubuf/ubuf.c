#include "ubuf.h"

ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
  size_t bytes_rem = n;
  size_t bytes_read = 0;

  char *bufp = usrbuf;

  // necessary to handle short counts
  while (bytes_rem) {
    if (bytes_read = read(fd, bufp, bytes_rem) < 0) {
      // Interrupted by signal handler. Retry read.
      if (errno == EINTR) {
        bytes_read = 0;
        continue;
      }

      // Fatal error.
      return -1;
    } else if (bytes_read == 0) {
      // Reached EOF
      break;
    }

    // Increment the buffer pointer by the bytes read and adjust the remaining
    // number of bytes to be read.
    bufp += bytes_read;
    bytes_rem -= bytes_read;
  }

  // If this is nonnegative and != 0, then there's been a short count.
  return (n - bytes_read);
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n) {
  size_t bytes_rem = n;
  size_t bytes_written = 0;

  char *bufptr = usrbuf;

  while (bytes_rem) {
    // unlike readn, writing 0 bytes is an error
    if (bytes_written = write(fd, usrbuf, n) <= 0) {
      if (errno == EINTR) {
        bytes_written = 0;
        continue;
      }

      return -1;
    }

    bufptr += bytes_written;
    bytes_rem -= bytes_written;
  }

  return n;
}

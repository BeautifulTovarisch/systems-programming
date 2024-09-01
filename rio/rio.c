#include "rio.h"

void rio_readinitb(rio_t *rp, int fd) {
  rp->fd = fd;
  rp->cnt = 0;
  rp->bufptr = rp->buf;
}

ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
  size_t bytes_rem = n;
  size_t bytes_read = 0;

  char *bufp = usrbuf;

  // necessary to handle short counts
  while (bytes_rem) {
    if ((bytes_read = read(fd, bufp, bytes_rem)) < 0) {
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
    if ((bytes_written = write(fd, usrbuf, n)) <= 0) {
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

// TODO: Should draw a picture of what's happening here.
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
  // Fill up the buffer
  while (rp->cnt <= 0) {
    rp->cnt = read(rp->fd, rp->buf, sizeof(rp->buf));

    if (rp->cnt < 0) {
      if (errno == EINTR) {
        return -1;
      }
    }

    // EOF
    if (rp->cnt == 0) {
      return 0;
    }

    // Successfully filled buffer. Point the bufptr to the next unread byte
    rp->bufptr = rp->buf;
  }

  int cnt = n;
  if (rp->cnt < n) {
    cnt = rp->cnt;
  }

  // Copy [rp->cnt] bytes from the rio buffer into the target.
  memcpy(usrbuf, rp->bufptr, cnt);

  // Bookkeeping
  rp->cnt -= cnt;
  rp->bufptr += cnt;

  return cnt;
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
  // Bytes to read and Read count.
  int n, rc;

  // The byte to be read.
  char c;
  char *line = usrbuf;

  // [n] will read at most [maxlen-1] characters.
  for (n = 1; n < maxlen; n++) {
    // read one byte at a time until reaching '\n'.
    if ((rc = rio_read(rp, &c, 1)) == 1) {
      // Byte successfully read. Write to next character in line.
      *line++ = c;

      // Reached terminating newline character.
      if (c == '\n') {
        n++;

        break;
      }
    } else if (rc == 0) {
      // EOF. Since n is one, we haven't read any bytes.
      if (n == 1) {
        return 0;
      } else {
        // EOF. All bytes successfully read.
        break;
      }
    }

    // All other possibilities are an error.
    return -1;
  }

  // Point to the beginning of the string.
  *line = 0;

  return n - 1;
}

// Similar to readlineb, except no arbitrary splitting at lines.
ssize_t rio_readb(rio_t *rp, void *usrbuf, size_t n) {
  size_t bytes_rem = n;
  size_t bytes_read = 0;

  char *bufptr = usrbuf;

  while (bytes_rem) {
    if ((bytes_read = rio_read(rp, bufptr, bytes_rem)) < 0) {
      return -1;
    } else if (bytes_read == 0) {
      break;
    }

    bufptr += bytes_read;
    bytes_rem -= bytes_read;
  }

  return (n - bytes_rem);
}

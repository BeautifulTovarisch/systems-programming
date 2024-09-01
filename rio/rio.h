/** Robust I/O
 *
 * This package provides robust implementations of buffered and unbuffered I/O.
 */

#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef RIO_H_
#define RIO_H_
#define RIO_BUFSIZE (1 << 13)

/** rio_t maintains information about an internal buffer used in buffered I/O
 * operations.
 *                |-  cnt -|
 *  [already read][ unread ][ ... ]
 *  ^             ^
 *  buf           buftpr
 */
typedef struct rio_t {
  char *bufptr;          // The next unread byte
  int fd;                // The file descriptor
  int cnt;               // Unread bytes in buffer
  char buf[RIO_BUFSIZE]; // A buffer used in buffered operations
} rio_t;

// rio_readinitb initalizes a rio_t structure.
void rio_readinitb(rio_t *rp, int fd);

// rio_readn reads up to [n] bytes into [usrbuf] returning the number of bytes
// remaining to be read or -1 if an error occurs.
ssize_t rio_readn(int fd, void *usrbuf, size_t n);

// rio_writen writes up to [n] bytes from [usrbuf] into the file at [fd]. This
// function returns [n] if all bytes were written or -1 if an error occurs.
ssize_t rio_writen(int fd, void *usrbuf, size_t n);

// rio_readlineb reads the next line from [rp->fd] into [usrbuf]. The function
// reads AT MOST [maxlen - 1] bytes (leaving a byte for NULL). Lines read into
// [usrbuf] are terminated by NULL.
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

// rio_readnb reads up to [n] bytes from [rp->fd] into [usrbuf].
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);

#endif

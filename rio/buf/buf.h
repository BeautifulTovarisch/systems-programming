/** RIO Buf
 *
 * This module provides buffered I/O functions suitable for reading and writing
 * application data.
 */

#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#ifndef RIO_BUF_H_
#define RIO_BUF_H_

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

// rio_readelineb reads the next line from [rp->fd] into [usrbuf]. The function
// reads AT MOST [maxlen - 1] bytes (leaving a byte for NULL). Lines read into
// [usrbuf] are terminated by NULL.
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

// rio_readnb reads up to [n] bytes from [rp->fd] into [usrbuf].
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);

#endif

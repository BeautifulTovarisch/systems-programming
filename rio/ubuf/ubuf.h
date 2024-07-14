/** RIO Unbuffered
 *
 * This module implements unbuffered reads and write operations.
 */

#ifndef RIO_UBUF_H_
#define RIO_UBUF_H_

#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

// rio_readn reads up to [n] bytes into [usrbuf] returning the number of bytes
// remaining to be read or -1 if an error occurs.
ssize_t rio_readn(int fd, void *usrbuf, size_t n);

// rio_writen writes up to [n] bytes from [usrbuf] into the file at [fd]. This
// function returns [n] if all bytes were written or -1 if an error occurs.
ssize_t rio_writen(int fd, void *usrbuf, size_t n);

#endif

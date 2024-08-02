/** MMap Concatenate
 *
 * This program copies a file on disk to STDOUT. The implementation uses mmap
 * instead of reading the contents of the file into memory and then writing out
 * to the file descriptor.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage: %s <file>\n", argv[0]);

    return 0;
  }

  int fd;
  if ((fd = open(argv[1], O_RDONLY, 0)) < 0) {
    fprintf(stderr, "error opening file: %s\n", strerror(errno));

    return 1;
  }

  // Get the size of the open file to provide to mmap.
  struct stat fi;
  if (fstat(fd, &fi) < 0) {
    fprintf(stderr, "error opening file: %s\n", strerror(errno));

    return 1;
  }

  size_t size = fi.st_size;
  // Map the contents of [fd] into a buffer the size of the file.
  char *bufp = mmap(NULL, fi.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

  // Write the contents of the file to stdout
  if (write(STDOUT_FILENO, bufp, size) < 0) {
    fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));

    return 1;
  }

  return 0;
}

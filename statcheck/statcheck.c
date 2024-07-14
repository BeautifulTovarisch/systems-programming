/** Statcheck
 *
 * This program outputs some information about a file obtained by a call to the
 * stat system procedure.
 *
 * usage: ./statcheck <file>
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  struct stat fileinfo;
  char *type = "other";
  char *readok = "no";

  if (argc < 2) {
    fprintf(stderr, "usage: %s <fileno>\n", argv[0]);

    return 1;
  }

  int code;
  if (code = stat(argv[1], &fileinfo) < 0) {
    fprintf(stderr, "error opening file: %s\n", strerror(errno));

    return 1;
  }

  // S_IFMT masks off the file type bits from st_mode.
  switch (fileinfo.st_mode & S_IFMT) {
  case S_IFREG:
    type = "regular";

    break;
  case S_IFDIR:
    type = "directory";
  }

  // Check whether the user has permissions to read the file.
  if (fileinfo.st_mode & S_IRUSR) {
    readok = "yes";
  }

  printf("File: %s. (type=%s, readok=%s)\n", argv[1], type, readok);

  return 0;
}

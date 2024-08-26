/** Hex to Dotted Decimal
 *
 * This program accepts a 4-byte hexadecimal integer and converts it to dotted
 * decimal format. The hexadecimal integer must be provided in host byte order.
 *
 * USAGE
 *  ./hextodd <hexadecimal integer>
 *
 * EXAMPLE
 *  ./hextodd 0x8002c2f2
 *  128.2.194.242
 */

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// attempt to parse arguments, checking errno for failures. For simplicity, we
// just terminate rather than provide any sophisticated error propagation.
long readargs(char *input) {
  int old = errno;
  errno = 0;

  char *end;
  long addr = strtol(input, &end, 16);
  if (errno != 0) {
    fprintf(stderr, "error parsing address: %s\n", strerror(errno));

    exit(EXIT_FAILURE);
  }

  errno = old;

  return addr;
}

int main(int argc, char *argv[argc]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <hexadecimal integer>\n", argv[0]);

    return EXIT_FAILURE;
  }

  // Attempt to read in hexadecimal integer.
  long addr = readargs(argv[1]);

  // Convert to network byte order.
  uint32_t net_addr = ntohl(addr);

  char dotted[16];
  if (!inet_ntop(AF_INET, &net_addr, dotted, 32)) {
    fprintf(stderr, "error converting to presentation format: %s\n",
            strerror(errno));

    return EXIT_FAILURE;
  }

  printf("%s\n", dotted);
}

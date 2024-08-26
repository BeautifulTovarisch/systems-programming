/** Dotted Decimal To Hex
 *
 * This program accepts a dotted-decimal IP address and returns its hexadecimal
 * representation.
 *
 * USAGE:
 *  ./dd2hex ddd.ddd.ddd.ddd
 *
 * EXAMPLE:
 *  ./dd2hex 127.0.0.1
 *  0x7f000001
 */

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: Need to read Richard Stevens for some error-handling advice.
void convert_addr(char *input, uint32_t *out) {
  int success = inet_pton(AF_INET, input, out);

  if (!success) {
    fprintf(stderr, "invalid IPv4 address\n");

    return;
  }

  if (success < 0) {
    fprintf(stderr, "error parsing input: %s\n", strerror(errno));

    return;
  }
}

int main(int argc, char *argv[argc]) {
  if (argc < 2) {
    fprintf(stderr, "%s ddd.ddd.ddd.ddd\n", argv[0]);

    return EXIT_FAILURE;
  }

  uint32_t net_order;
  // First convert from presentation to binary
  convert_addr(argv[1], &net_order);

  // inet_pton writes the address in network order, so we need to convert to
  // host order.
  uint32_t host_order = ntohl(net_order);

  printf("%#x\n", host_order);

  return EXIT_SUCCESS;
}

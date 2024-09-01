/** Get Host Info
 *
 * This program demonstrates the basic usage of the getaddrinfo and getnameinfo
 * functions. The main routine simply displays information about a domain name
 * provided as a command-line argument.
 *
 * NOTE: Because getaddrinfo is not part of the C standard, this program must be
 * compiled with -D_POSIX_C_SOURCE=200112L
 *
 * USAGE:
 *
 *  ./gethostinfo <domain name>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE (1 << 13)

int main(int argc, char *argv[argc]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <domain name>\n", argv[0]);

    return EXIT_FAILURE;
  }

  struct addrinfo *listp, hints;

  // Initialize hints structure and configure
  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int rc;
  if ((rc = getaddrinfo(argv[1], (void *)0, &hints, &listp)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));

    return EXIT_FAILURE;
  }

  // Used to store the human-readable name of the host.
  char hostbuf[MAXLINE];

  // Traverse the linked list and display each IP address
  for (struct addrinfo *p = listp; p; p = p->ai_next) {
    if ((rc = getnameinfo(p->ai_addr, p->ai_addrlen, hostbuf, MAXLINE,
                          (void *)0, 0, NI_NUMERICHOST)) != 0) {
      fprintf(stderr, "getnameinfo error: %s\n", gai_strerror(rc));

      return EXIT_FAILURE;
    }

    printf("%s\n", hostbuf);
  }

  freeaddrinfo(listp);

  return EXIT_SUCCESS;
}

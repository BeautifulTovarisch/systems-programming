/** Echo Client
 *
 * This program is a simple client of the echo server. The only function of this
 * client is to write whatever bytes are provided on the command line to a the
 * echo server and print out the response. The host and port of the server are
 * also provided on the command line.
 *
 * USAGE:
 *  ./echo <host> <port>
 *
 * Please install the rio and socket-helper libraries before compiling.
 *
 * TODO: Review the chapter on linking and figure out the best way to do this
 * automatically.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rio.h>
#include <socket-helper.h>

int main(int argc, char *argv[argc]) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);

    return EXIT_FAILURE;
  }

  char *host = argv[1];
  char *port = argv[2];

  // This stupid function should be called "connect_to_server"
  int clientfd = open_clientfd(host, port);
  if (clientfd < 0) {
    fprintf(stderr, "error connecting to %s:%s: %s\n", host, port,
            strerror(errno));

    return EXIT_FAILURE;
  }

  // Initialize a rio struct for buffered reading below
  rio_t rio;
  rio_readinitb(&rio, clientfd);

  char buf[MAXLINE];
  while ((fgets(buf, MAXLINE, stdin)) != (void *)0) {
    if (ferror(stdin)) {
      fprintf(stderr, "error reading from stdin\n");

      return EXIT_FAILURE;
    }

    // Write to clientfd. Notice this is an unbuffered write
    if ((rio_writen(clientfd, buf, strlen(buf))) < 0) {
      fprintf(stderr, "error writing to client\n");

      return EXIT_FAILURE;
    }

    rio_readlineb(&rio, buf, MAXLINE);
    fputs(buf, stdout);
  }

  return EXIT_SUCCESS;
}

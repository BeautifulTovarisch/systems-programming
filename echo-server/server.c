/** Echo Server
 *
 * This server binds to a port provided on the command line and listens for
 * connections. The only functionality of the server is to echo back out what
 * the client sends it.
 *
 * USAGE:
 *
 *  ./echoserv <port>
 *
 * This program requires the socket-helper and rio packages and
 * -D_POSIX_C_SOURCE=200112L
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <rio.h>
#include <socket-helper.h>

// Echo back whatever the client has sent
void echo(int conn) {
  char buf[MAXLINE];

  rio_t rio;
  rio_readinitb(&rio, conn);

  size_t n;
  while ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    printf("server received %ld bytes\n", n);

    // Ignore this error. Need to think about a good way to handle this.
    rio_writen(conn, buf, strlen(buf));
  }
}

int main(int argc, char *argv[argc]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);

    return EXIT_FAILURE;
  }

  char *port = argv[1];

  // Bind and listen for connections
  int serverfd = open_listenfd(port);
  if (serverfd < 0) {
    fprintf(stderr, "error listening on port %s\n", port);

    return EXIT_FAILURE;
  }

  struct sockaddr_storage clientaddr;
  char client_host[MAXLINE], client_port[MAXLINE];

  while (true) {
    socklen_t clientlen = sizeof(struct sockaddr_storage);
    int conn = accept(serverfd, (struct sockaddr *)&clientaddr, &clientlen);
    if (conn < 0) {
      fprintf(stderr, "error accepting connection: %s\n", strerror(errno));

      return EXIT_FAILURE;
    }

    int rc = getnameinfo((struct sockaddr *)&clientaddr, clientlen, client_host,
                         MAXLINE, client_port, MAXLINE, 0);

    if (rc != 0) {
      fprintf(stderr, "getnameinfo error: %s\n", gai_strerror(rc));

      return EXIT_FAILURE;
    }

    printf("Connected to (%s, %s)\n", client_host, client_port);

    echo(conn);

    if ((close(conn) < 0)) {
      fprintf(stderr, "error closing connection: %s\n", strerror(errno));

      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

/** Tiny HTTP
 *
 * This program is a minimal HTTP/1.0 server built to tie together concepts such
 * as I/O and exceptional control flow. The server is sequential, serving
 * exactly one client at a time.
 *
 * Tiny listens on a port provided on the command line and support only the GET
 * HTTP verb on the "/" endpoint. This implementation makes no attempt to secure
 * communication or provide any other features common in modern web servers.
 *
 * USAGE
 *  ./tinyhttp <port>
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <rio.h>
#include <socket-helper.h>

// serve should not return until normal circumstances.
static int serve(char *port) {
  struct sockaddr_storage clientaddr;

  // char client_host[MAXLINE], client_port[MAXLINE];

  int serverfd = open_listenfd(port);
  if (serverfd < 0) {
    return -1;
  }

  while (true) {
    socklen_t clientlen = sizeof(clientaddr);
    int conn = accept(serverfd, (struct sockaddr *)&clientaddr, &clientlen);
    if (conn < 0) {
      return -1;
    }

    // TODO: handle HTTP request.

    if (close(conn) < 0) {
      return -1;
    }
  }

  // Some critical failure has occurred.
  return -1;
}

int main(int argc, char *argv[argc]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);

    return EXIT_FAILURE;
  }

  char *port = argv[1];

  // Should never return under normal operation.
  serve(port);

  fprintf(stderr, "critical failure. exiting...\n");

  return EXIT_FAILURE;
}

#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "socket-helper.h"

int open_clientfd(char *hostname, char *port) {
  struct addrinfo *addrlist, hints;

  // Initialize and configure hints
  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_flags = AI_NUMERICSERV | AI_ADDRCONFIG;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(hostname, port, &hints, &addrlist)) {
    // TODO: Develop proper error constructs
    return -1;
  }

  // Loop through the linked list until able to establish a connection. This
  // attempts two operations which may fail:
  //  1. Opening a socket
  //  2. Connecting to the server
  //
  // The loop continues until either all options are exhausted or a successful
  // connection is established. No error is propagated if a socket fails to open
  // or connect.

  int clientfd;
  for (struct addrinfo *p = addrlist; p; p = p->ai_next) {
    if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
      continue;
    }

    // Connect to the server
    if ((connect(clientfd, p->ai_addr, p->ai_addrlen)) != -1) {
      freeaddrinfo(addrlist);

      return clientfd;
    }

    // Ignoring error here. Bad practice.
    close(clientfd);
  }

  // All connections have failed, clean up and return an error.
  freeaddrinfo(addrlist);

  return -1;
}

int open_listenfd(char *port) {
  struct addrinfo *addrlist, hints;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;

  if (getaddrinfo((void *)0, port, &hints, &addrlist)) {
    return -1;
  }

  // Similar to openclientfd; iterate until the first successful call to bind()
  int listenfd;
  for (struct addrinfo *p = addrlist; p; p = p->ai_next) {
    if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
      continue;
    }

    // Once again ignoring errors. Need to review a more advanced networking
    // book to understand this.
    int optval = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,
               sizeof(int));

    if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) {
      freeaddrinfo(addrlist);

      // Prepare listenfd for listening as server.
      if (listen(listenfd, LISTENQ) < 0) {
        return -1;
      }

      return listenfd;
    }

    close(listenfd);
  }

  freeaddrinfo(addrlist);

  return -1;
}

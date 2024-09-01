/** Socket Helper
 *
 * This module wraps the getaddrinfo and getnameinfo functions to provide a
 * simpler interface. This is especially helpful for building web servers.
 *
 * Programs using this module must be compiled with -D_POSIX_C_SOURCE=200112L
 */

#ifndef SOCKET_HELPER_H
#define SOCKET_HELPER_H

// LISTENQ defines the queue size provided to listen()
#define LISTENQ (1 << 10)

// TODO: Figure out how to accurately document this
#define MAXLINE (1 << 13)

// open_clientfd attempts to connect to a server described by [hostname]:[port].
// If successful, the function returns the file descriptor of the client socket,
// and -1 otherwise.
int open_clientfd(char *hostname, char *port);

// open_listenfd attempts to listen for connections on [port]. If successful, a
// file descriptor ready to receive connections is returned, with -1 returned on
// an error.
int open_listenfd(char *port);

#endif

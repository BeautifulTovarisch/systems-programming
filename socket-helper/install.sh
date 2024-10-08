# This script compiles and installs the socket-helper library to /usr/local/lib.
# Clients should use:
#
#   #include <socket-helper.h>
#
# in order to import these utilities.

#!/bin/bash

set -euo pipefail

# Compile
c99 -Wall -c socket-helper.c -D_POSIX_C_SOURCE=200112L

# Archive the object file 'socket-helper.o'
ar rcs socket-helper.a socket-helper.o

# Install to /usr/local/* with appropriate permission
install -m 0644 socket-helper.a /usr/local/lib
install -m 0644 socket-helper.h /usr/local/include

# Cleanup
rm *.a *.o

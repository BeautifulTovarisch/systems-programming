# This script compiles and installs the rio helper library to /usr/local/lib.
# Clients should use:
#
#   #include <rio.h>
#
# in order to import these utilities.

#!/bin/bash

set -euo pipefail

# Compile
c99 -Wall -c rio.c

# Archive the object file 'rio.o'
ar rcs rio.a rio.o

# Install to /usr/local/lib with appropriate permission
mkdir -p /usr/local/lib/rio
install -m 0644 rio.a /usr/local/lib/rio/

# Cleanup
rm *.a *.o

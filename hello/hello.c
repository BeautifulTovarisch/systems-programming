/**
 * Hello, World
 *
 * Most people's very first C program. Included here and annotated to all hell
 * to serve as an introduction to systems programming. This directory includes
 * a representative example of documentation structure and a reference Makefile
 * for posterity.
 */

// This is an include directive. stdio.h is a header file, which declares the
// public interface of a C library. This particular library comes with almost
// all systems and is usually located under /usr/include or some other standard
// location.
//
// The stdio library provides the printf function, which encapsulates a lot of
// complicated I/O in order to print a message to STDOUT. Unfortunately in C,
// the interface of a library is not appparent from the import, meaning the
// programmer has to rely on good documentation (or read the source) to know
// know what functions and data a library provides.
//
// There are man pages for many of the standard library functions.
//
//  man stdio
//
// Gives the man page for this library in particular.
#include <stdio.h>

// This is the main function, the entry point for a C application. Before name
// of the function comes the return type, in this case: 'int'. The return type
// of main needs to be an integer in order to communicate the exit status of
// the program to the shell (or some other program).
//
// In general, anything other than 0 means something unexpected has occurred.
int main() {
  // This function compactly hides several operations:
  //  1. Opening a file (STDOUT)
  //  2. Formatting optional arguments (not shown here)
  //  3. Writing the formatted message to STDOUT
  //  4. Closing the file
  //
  // More information about printf can be found with:
  //
  //  man 3 printf
  printf("hello, world\n");

  // Finally, returning 0 here indicates to the shell or downstream programs
  // that this program has exited successfully. If a C program needs to return
  // an error, a common strategy is to enumerate a set of error codes that can
  // be used by a client program. The error codes can then be interpreted based
  // on the meaning found in the documentation.
  return 0;
}

/** Exchange
 *
 * This simple program contains a single function which exchanges two values.
 * The purpose of the function is to establish the connection between memory
 * movement operations and C source code.
 */

// exchange sets the value contained at [xp] to [y] and the value of [y] to the
// value at [xp].
//
// This produces the following assembly output:
//  movq    (%rdi), %rax  # Move the first argument to the return register
//  movq    %rsi, (%rdi)  # Move the 2nd arg into the memory address of the 1st
//  ret
long exchange(long *xp, long y) {
  long tmp = *xp;
  *xp = y;

  // Notice there is no point in this assignment, so the compiler removes it.
  y = tmp;

  return tmp;
}

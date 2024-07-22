/** Non Local Jumps
 *
 * This program contains two non-local jumps which "return" from procedures in
 * the event of a simulated error. This behavior is the underlying mechanism
 * behind modern "exceptions" found in higher-level programming languages.
 *
 * Much like exceptions, this behavior can be abused and quickly complicate a
 * program to the point of illegibility. Consider alternatives before reaching
 * for this construct.
 */

#include <setjmp.h>
#include <stdio.h>

// Data structure to record the location of "jump points" and other state.
jmp_buf buf;

int error = 0;

void proc_b() {
  if (error) {
    longjmp(buf, 2);
  }
}

// This procedure registers a jump destination at 1 and toggles the value of
// error so that a check proc_b() will trigger a jump to case 2.
void proc_a() {
  if (!error) {
    error = 1;

    proc_b();
  }

  // if proc_b is run, the program will terminate before executing this line.
  longjmp(buf, 1);
}

// Main consists of a switch on setjmp(), which saves information about the
// calling environment for use after longjmp() transfers control to an nonlocal
// location in a procedure.
int main(int argc, char *argv[]) {
  // The cases in this switch establish destinations longjmp().
  switch (setjmp(buf)) {
  // This initial case is usually triggered when setjmp() initializes the jmp
  // buffer.
  case 0:
    proc_a();
    break;
  case 1:
    printf("proc_a() error\n");
    break;
  case 2:
    printf("proc_b() error\n");
    break;
  default:
    printf("invalid jump destination reached\n");

    return 1;
  }

  return 0;
}

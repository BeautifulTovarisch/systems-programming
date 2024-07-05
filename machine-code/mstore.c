/** Multiply and Store
 *
 * This program is meant to be a simple example that demonstrates how machine
 * code output from the assembler relates to the C source code.
 *
 * The following commands are helpful:
 *
 * ASSEMBLE
 *  gcc -Og -S mstore.c
 *
 * COMPILE
 *  gcc -Og -c mstore.c
 *
 * DISASSEMBLE
 *  objdump -d mstore.o
 */

long mult2(long, long);

// multstore computes the value mult2(x, y) and stores the result in [dest].
void multstore(long x, long y, long *dest) {
  long result = mult2(x, y);

  *dest = result;
}

/** My Args
 *
 * This program outputs its arguments and environment variables following the
 * format:
 *
 * Arguments:
 *  argv[0]: <1st argument>
 *  argv[1]: <2nd argument>
 *  ...
 *
 * Environment:
 *  envp[0]: <key>=<value>
 *  envp[1]: <key>=<value>
 *  ...
 */

#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
  printf("Arguments:\n");
  for (int i = 0; i < argc; i++) {
    printf("\targv[%d]: \t%s\n", i, argv[i]);
  }

  printf("Environment:\n");
  for (int i = 0; envp[i] != NULL; i++) {
    printf("\tenvp[%d]: \t%s\n", i, envp[i]);
  }

  return 0;
}

/** Microshell
 *
 * This program is a bare-minimum shell. The main loop outputs a prompt which
 * accepts user input. The input is parsed as a command line and run as a child
 * process if it is not a built-in command.
 *
 * The program demonstrates the interplay between exec() and fork() as well as
 * the basic structure of a shell.
 *
 * usage: microshell <command> [args]
 */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#define MAXARGS (1 << 7)
#define MAXLINE (1 << 13)

extern char **environ;

// cmd encapsulates the arguments and corresponding argument count as well as
// whether the command should be run as a background process.
typedef struct cmd {
  char **argv;
  int argc;
  char bg;
} cmd;

// trimspaces advances [buf] past any leading spaces (' ').
void trimspaces(char *buf) {
  char *b = buf;
  while (*b && (*b == ' ')) {
    b++;
  }

  *buf = *b;
}

void print_args(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    printf("argv[%d]=%s\n", i, argv[i]);
  }
}

// parseline reads [buf] and produces a [cmd] struct containing information
// about the input arguments.
struct cmd *parseline(char *buf) {
  // Number of arguments
  int argc = 0;

  // To avoid repeated writes to memory, this is copied to command->argv at the
  // end of this function.
  char *argv[MAXARGS];

  struct cmd *command = malloc(sizeof(struct cmd));
  command->argv = malloc(MAXARGS * sizeof(char *));

  // terminating '\n' replaced with empty space
  buf[strlen(buf) - 1] = ' ';

  trimspaces(buf);

  // Set delim to the first instance of a space in [buf]. This combined with
  // repeatedly 'trimmming' leading spaces has the following effect:
  //
  //  argument1 argument2
  //  ^        ^
  //  buf      delim
  //
  // effectively fencing each argument as [buf] is processed.
  char *delim;
  while ((delim = strchr(buf, ' '))) {
    argv[argc++] = buf;

    // Set delimiter to NULL and advance [buf] 1 character past.
    *delim = '\0';
    buf = delim + 1;

    trimspaces(buf);
  }

  // Terminate argument list
  argv[argc] = NULL;

  print_args(argc, argv);

  // Check the final argument for a single ampersand '&' indicating whether the
  // program should be run as a background job.
  //
  // Example:
  //  argument1 argument2 &
  //                    ^
  //                    arguments trimmed to this point, job run in background
  if ((command->bg = (*argv[argc - 1] == '&'))) {
    // Terminate just before the '&'
    argv[--argc] = NULL;
  }

  command->argc = argc;
  memcpy(command->argv, argv, sizeof(char *) * argc);

  return command;
}

// eval evaluates the [cmdline] provided.
void eval(char *cmdline) {
  struct cmd *command = parseline(cmdline);

  // No arguments. Nothing to do.
  if (!command->argc) {
    free(command->argv);
    free(command);

    return;
  }

  pid_t pid;
  // Run the command as a child process
  if ((pid = fork()) < 0) {
    fprintf(stderr, "error forking child: %s\n", strerror(errno));
    exit(1);
  }

  if (pid == 0) {
    execve(command->argv[0], command->argv, environ);

    fprintf(stderr, "error executing '%s': %s\n", command->argv[0],
            strerror(errno));
    exit(1);
  }

  int status;
  // If not run as a background job, the parent process waits for the child
  if (!command->bg) {
    if (waitpid(pid, &status, 0) < 0) {
      fprintf(stderr, "waitpid error: %s", strerror(errno));
      exit(status);
    }
  } else {
    printf("[%d]: %s\n", pid, cmdline);
  }

  free(command->argv);
  free(command);
}

int main(int argc, char *argv[]) {
  char cmdline[MAXLINE];

  while (1) {
    // Basic prompt.
    printf("$ ");

    if (fgets(cmdline, MAXLINE, stdin) == NULL) {
      // No input to process?
      if (feof(stdin)) {
        return 0;
      }

      fprintf(stderr, "error reading input. Terminating...");

      return 1;
    }

    eval(cmdline);
  }

  return 0;
}

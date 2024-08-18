/** Enumerations
 *
 * This program demonstrates declaring an enumeration and using its members to
 * as keys in an association with string literals. This is somewhat nuanced, so
 * I wanted to have an annotated example for reference.
 */

#include <stdio.h>

// Macros may also be used to declare a compound literal. I prefer to keep them
// at the very top of the file and use all-caps.
enum direction { Up, Down, Left, Right, direction_size };

// The syntax of compound literals is of the form: (T){ INIT }
// There are several rules/good practices when declaring compound literals:
//
// 1. The type should be const-qualified to allow for optimization
// 2. There needs to be space between the name and the () to avoid declaring a
//    function-like macro.
// 3. Backslashes are used to continue the macro over several lines
// 4. The definition must not include a terminating semicolon.
#define DIRECTIONS /**/                                                        \
  (char const *const[direction_size]) {                                        \
    [Up] = "Up", [Down] = "Down", [Left] = "Left", [Right] = "Right"           \
  }

// Declare an enum of colors. The num_colors enumration is a clever way to let
// clients iterate through the color enumeration sequentially.
enum color { Red, Blue, Green, Yellow, num_colors };

// Used in this way, the abstraction of an enum allows O(1) lookups of string
// representations of the corresponding color. Here [num_colors] is once again
// used to ensure the enumeration can grow in a way that minimizes programmer
// errors.
char const *const color_map[num_colors] = {
    [Red] = "Red", [Blue] = "Blue", [Green] = "Green", [Yellow] = "Yellow"};

int main(int argc, char *argv[argc]) {
  // This expresses each member of the enumeration.
  for (unsigned i = 0; i < num_colors; i++)
    printf("color[%u] = %s\n", i, color_map[i]);

  // Using the equivalent macro-defined lookup table.
  for (unsigned i = 0; i < direction_size; i++)
    printf("direction[%u] = %s\n", i, DIRECTIONS[i]);
}

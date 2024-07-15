#include <stdio.h>

int main() {
  char *s_ptr = "Hello, World!";
  char s_arr[] = "Hello, World!";

  // Test immutability of string literals
  s_arr[0] = 'k'; // This is allowed
  // s_ptr[0] = 'k'; // This is not allowed. What would happen? Try it!

  printf("s: %c\n", s_arr[0]);
}

#include "interpretcmd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  char* cmd[] = {"cd", "doc/"};

  char* result = read_cmd_line(cmd);
  while(result != NULL) {
    printf("%s\n", result);
  }
  return 0;
}

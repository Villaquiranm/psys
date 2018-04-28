#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// SOME CHECKS OMMITTED!

// helper for testing, not necessary for strtok()
static char *strduplicator(const char *s)
{
  char *dup;
  dup = malloc(strlen(s) + 1);
  if (dup != NULL) {
    strcpy(dup, s);
  }
  return dup;
}


// thread-safe (sort of) version
char *my_strtok(char *in, const char *delim, char **pos)
{
  char *token = NULL;

  // if the input is NULL, we assume that this
  // function run already and use the new position
  // at "pos" instead
  if (in == NULL) {
    in = *pos;
  }
  // skip leading delimiter that are left
  // there from the last run, if any
  in += strspn(in, delim);
  // if it is still not the end of the input
  if (*in != '\0') {
    // start of token is at the current position, set it
    token = in;
    // skip non-delimiters, that is: find end of token
    in += strcspn(in, delim);
    // strip of token by setting first delimiter to NUL
    // that is: set end of token
    if (*in != '\0') {
      *in = '\0';
      in++;
    }
  }
  // keep current position of input in "pos"
  *pos = in;
  return token;
}

static char *pos;
char *own_strtok(char *in, const char *delim)
{
   return my_strtok(in, delim, &pos);
}

int main(void)
{

  char *in_1 = strduplicator("this,is;the:test-for!strtok.");
  char *in_2 = strduplicator("this,is;the:test-for!my_strtok.");

  char *position, *token, *s_in1 = in_1, *s_in2 = in_2;
  const char *delimiters = ",;.:-!";

  token = strtok(in_1, delimiters);
  printf("BUILDIN: %s\n", token);
  for (;;) {
    token = strtok(NULL, delimiters);
    if (token == NULL) {
      break;
    }
    printf("BUILDIN: %s\n", token);
  }

  token = my_strtok(in_2, delimiters, &position);
  printf("OWNBUILD: %s\n", token);
  for (;;) {
    token = my_strtok(NULL, delimiters, &position);
    if (token == NULL) {
      break;
    }
    printf("OWNBUILD: %s\n", token);
  }

  free(s_in1);
  free(s_in2);

  exit(EXIT_SUCCESS);
}

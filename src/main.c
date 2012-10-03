#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

FILE *sourcecode;

int main(int argc, char **argv) {

  char input[600];

  sourcecode = fmemopen (input, strlen(input), "r");
  lookahead = gettoken(sourcecode);
  program();

  return 0;
}


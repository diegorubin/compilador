#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

FILE *sourcecode;

int main(int argc, char **argv) {

  char input[600];

  scanf("%[^\n]", input);
  printf("= ");

  sourcecode = fmemopen (input, strlen(input), "r");
  lookahead = gettoken(sourcecode);
  mybc();

  return 0;
}


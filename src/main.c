#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

FILE *sourcecode;

int main(int argc, char **argv) {

  if (argc > 1)
    sourcecode = fopen(argv[1], "r");
  else
    sourcecode = stdin;

  lookahead = gettoken(sourcecode);
  mybc();

  printf("\n");

  return 0;
}


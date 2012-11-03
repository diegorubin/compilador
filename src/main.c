#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

FILE *sourcecode;
FILE *target;

int main(int argc, char **argv) {

  sourcecode = stdin;
  target = open("source.out", "w");

  lookahead = gettoken(sourcecode);
  program();

  return 0;
}


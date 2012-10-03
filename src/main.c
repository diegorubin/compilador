#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

FILE *sourcecode;

int main(int argc, char **argv) {

  sourcecode = stdin;

  lookahead = gettoken(sourcecode);
  program();

  return 0;
}


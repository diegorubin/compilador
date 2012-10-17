#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/parser.h"
#include "tap.h"

FILE *sourcecode;

int main(int argc, char **argv)
{
  plan_tests(1);

  int token;
  char *input;

  input = "\
    PROGRAM seila;\
\
    VAR \
      a:INTEGER;\
      b,c,d:REAL;\
\ 
    BEGIN\
      a; \
      naodeclarado; \
      b \
    END.\
";

  sourcecode = fmemopen (input, strlen(input), "r");

  lookahead = gettoken(sourcecode);
  program();

  ok1("Accepted Language");

  return exit_status();
}


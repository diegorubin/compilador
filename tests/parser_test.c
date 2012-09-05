#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "../src/parser.h"
#include "tap.h"

FILE *sourcecode;

int main(int argc, char **argv)
{
  plan_tests(3);

  int token;
  char *input;

  input = "a + a\n";
  sourcecode = fmemopen (input, strlen(input), "r");

  lookahead = gettoken(sourcecode);
  mybc();

  ok1("Accepted Language");

  input = "(3 - a)\n";
  sourcecode = fmemopen (input, strlen(input), "r");

  lookahead = gettoken(sourcecode);
  mybc();

  ok1("Accepted Language");

  input = "- (a * a)\n";
  sourcecode = fmemopen (input, strlen(input), "r");

  lookahead = gettoken(sourcecode);
  mybc();

  ok1("Accepted Language");

  return exit_status();
}


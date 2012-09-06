#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../src/parser.h"
#include "tap.h"

FILE *sourcecode;

int main(int argc, char **argv)
{
  plan_tests(4);

  int token;
  char *input;

  char val[32];
  val[0] = 'a';
  val[1] = '\0';

  ok1(store(5, val) == 5.0);

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


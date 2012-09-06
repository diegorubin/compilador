#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/parser.h"
#include "tap.h"

FILE *sourcecode;

int main(int argc, char **argv)
{
  plan_tests(7);

  int token;
  char *input;

  char val[32];
  val[0] = 'a';
  val[1] = '\0';

  ok1(store(5, val) == 5.0);
  ok1(recall("a") == 5.0);
  ok1(recall("b") == .0);

  push(7);
  ok1(pop() == 7);

  input = "a + a\n";
  sourcecode = fmemopen (input, strlen(input), "r");

  lookahead = gettoken(sourcecode);
  mybc();

  ok1("Accepted Language");

  input = "(3 - 5)\n";
  sourcecode = fmemopen (input, strlen(input), "r");

  lookahead = gettoken(sourcecode);
  mybc();

  ok1("Accepted Language");

  input = "- (a + a) * 5\n";
  sourcecode = fmemopen (input, strlen(input), "r");

  lookahead = gettoken(sourcecode);
  mybc();

  ok1("Accepted Language");

  return exit_status();
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "../src/lexer.h"
#include "tap.h"

FILE *sourcecode;

int main(int argc, char **argv)
{
  plan_tests(1);

  int token;
  char *input;

  input = "abacaxi";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == ID);

  return exit_status();
}


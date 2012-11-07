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
  plan_tests(8);

  int token;
  char *input;

  input = "BEGIN";
  ok1(iskeyword(input) == BEGIN);

  input = "abacaxi";
  ok1(iskeyword(input) == 0);

  input = "abacaxi";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == ID);

  input = "END";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == END);

  input = "3455";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == UINT);

  input = ".3455";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == FLOAT);

  input = "1.3455";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == FLOAT);
  printf("lexeme: %s\n", lexeme);
  ok1(!strcmp(lexeme, "1.3455"));

  return exit_status();
}


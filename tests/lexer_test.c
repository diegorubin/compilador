#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <lexer.h>
#include "tap.h"

FILE *sourcecode;

int main(int argc, char **argv)
{
  plan_tests(12);

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
  ok1(!strcmp(lexeme, "1.3455"));

  input = "1.3455e3";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == FLOAT);
  ok1(!strcmp(lexeme, "1.3455E3"));

  input = "1 {comentario} 1";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == UINT);
  token = gettoken(sourcecode);
  ok1(token == UINT);

  return exit_status();
}


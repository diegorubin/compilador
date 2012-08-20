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
  plan_tests(15);

  int token;
  char *input;

  input = "abacaxi";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == ID);

  input = "1234";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == NUM); 

  input = "0";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == ZERO); 

  input = "0123";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == OCTA); 

  input = "0x123";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == HEXA); 

  input = "+";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == OPLUS); 

  input = "-";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == OPLUS); 

  input = "*";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == OTIMES); 

  input = "/";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == OTIMES); 

  input = ".12";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == FLOAT);

  input = ".12e17 abacaxi";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == FLOAT);
  token = gettoken(sourcecode);
  ok1(token == ID);

  input = "12.12";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == FLOAT);

  input = "12.12e12 abacaxi";
  sourcecode = fmemopen (input, strlen(input), "r");
  token = gettoken(sourcecode);
  ok1(token == FLOAT);
  token = gettoken(sourcecode);
  ok1(token == ID);


  return exit_status();
}


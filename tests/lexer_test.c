#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "../src/lexer.h"
#include "tap.h"

int main(int argc, char **argv)
{
  plan_tests(9);

  int token;
  FILE *buffer;
  char *input;

  input = "abacaxi";
  buffer = fmemopen (input, strlen(input), "r");
  token = gettoken(buffer);
  ok1(token == ID);

  input = "1234";
  buffer = fmemopen (input, strlen(input), "r");
  token = gettoken(buffer);
  ok1(token == NUM); 

  input = "0";
  buffer = fmemopen (input, strlen(input), "r");
  token = gettoken(buffer);
  ok1(token == ZERO); 

  input = "0123";
  buffer = fmemopen (input, strlen(input), "r");
  token = gettoken(buffer);
  ok1(token == OCTA); 

  input = "0x123";
  buffer = fmemopen (input, strlen(input), "r");
  token = gettoken(buffer);
  ok1(token == HEXA); 

  input = "+";
  buffer = fmemopen (input, strlen(input), "r");
  token = gettoken(buffer);
  ok1(token == OPLUS); 

  input = "-";
  buffer = fmemopen (input, strlen(input), "r");
  token = gettoken(buffer);
  ok1(token == OPLUS); 

  input = "*";
  buffer = fmemopen (input, strlen(input), "r");
  token = gettoken(buffer);
  ok1(token == OTIMES); 

  input = "/";
  buffer = fmemopen (input, strlen(input), "r");
  token = gettoken(buffer);
  ok1(token == OTIMES); 

  return exit_status();
}


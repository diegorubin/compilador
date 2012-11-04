#include <stdio.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "../src/parser.h"
#include "tap.h"

FILE *sourcecode;
FILE *target;

int main(int argc, char **argv)
{
  plan_tests(3);

  int token;
  char *input;

  sourcecode = fopen("exemplo.pas", "r");
  target = open("source.out", "w");

  lookahead = gettoken(sourcecode);
  program();

  ok1("Accepted Language");
  ok1(symtab_lookup("a"));
  ok1(symtab_lookup("vd"));

  return exit_status();
}


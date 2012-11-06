#include <stdio.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "../src/parser.h"
#include "tap.h"

FILE *sourcecode;
FILE *target;

void test_clearenv() 
{
  current_line = 1;
  lextape_nextentry = 0;
  symtab_nextentry = 1;
}

int main(int argc, char **argv)
{
  plan_tests(3);

  int token;
  char *input;

  /* Simple program with declarations of variables only */
  input = "\
    PROGRAM seila;\
\
    VAR \
      a:INTEGER;\
      b,c,d:REAL;\
\
    BEGIN\
    END.\
";

  sourcecode = fmemopen (input, strlen(input), "r");
  target = fopen("source.out", "w");

  lookahead = gettoken(sourcecode);
  program();

  test_clearenv();
  ok1("Simple program with declarations of variables only");

  /*Complete procedure declaration*/
  input = "\
    PROCEDURE proc;\
\
    VAR \
      a:INTEGER;\
      b,c,d:REAL;\
\
    BEGIN\
    END;\
";

  sourcecode = fmemopen (input, strlen(input), "r");
  target = fopen("source.out", "w");

  lookahead = gettoken(sourcecode);
  procedure();
  
  test_clearenv();
  ok1("Complete procedure declaration");

  /*Creating functions and procedures (module)*/
  input = "\
    FUNCTION conv(VAR x:INTEGER):REAL;\
\
    BEGIN\
      conv:= x;\
    END;\
\
    PROCEDURE proc;\
\
    VAR \
      a:INTEGER;\
      b,c,d:REAL;\
\
    BEGIN\
    END;\
";

  sourcecode = fmemopen (input, strlen(input), "r");
  target = fopen("source.out", "w");

  lookahead = gettoken(sourcecode);
  modules();
  
  test_clearenv();
  ok1("Creating functions and procedures (module)");

  return exit_status();
}


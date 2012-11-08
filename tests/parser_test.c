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

void fake_declaration(char const *identifier, int dtype, int idtype, int offset)
{
  symtab_insert(identifier, dtype, idtype, offset);
}

int main(int argc, char **argv)
{
  plan_tests(10);

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

  /* Cleaning variables of a module */
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
      x:INTEGER;\
\
    BEGIN\
      x := 3 + 4\
    END;\
";

  sourcecode = fmemopen (input, strlen(input), "r");
  target = fopen("source.out", "w");

  lookahead = gettoken(sourcecode);
  modules();
  
  /* variavel local da funcao nao pode ser acessada mais*/
  ok1(!symtab_lookup("X"));

  test_clearenv();
  ok1("Creating functions and procedures (module)");


  /*Assign expressions*/
  /* add x to symtab for text -> fake declaration */
  fake_declaration("X", INTEGER, SYMTAB_IDTYPE_VARIABLE, 0);
  input = "x := 3 + 4;";

  sourcecode = fmemopen (input, strlen(input), "r");
  target = fopen("source.out", "w");

  lookahead = gettoken(sourcecode);
  stmt();
  
  test_clearenv();
  ok1("Assign expressions");

  /*Multiple assign expressions*/
  fake_declaration("X", INTEGER, SYMTAB_IDTYPE_VARIABLE, 0);
  fake_declaration("Y", INTEGER, SYMTAB_IDTYPE_VARIABLE, 0);
  input = "\
    x := 3 + 4;\
    y := x + 4\
";

  sourcecode = fmemopen (input, strlen(input), "r");
  target = fopen("source.out", "w");

  lookahead = gettoken(sourcecode);
  stmtlist();
  
  test_clearenv();
  ok1("Multiple assign expressions");

  /*Boolean expression*/
  input = "3 + 4 < 10";

  sourcecode = fmemopen (input, strlen(input), "r");
  target = fopen("source.out", "w");

  lookahead = gettoken(sourcecode);
  expression();
  
  test_clearenv();
  ok1("Boolean expression");

  /*if stmt*/
  fake_declaration("X", INTEGER, SYMTAB_IDTYPE_VARIABLE, 0);

  input = "\
    IF 3 + 4 < 10 THEN\
      x:= 10\
    ELSE\
      x:= 1\
";

  sourcecode = fmemopen (input, strlen(input), "r");
  target = fopen("source.out", "w");

  lookahead = gettoken(sourcecode);
  ifstmt();
  
  test_clearenv();
  ok1("if stmt");

  /*if stmt case insensitive*/
  fake_declaration("X", INTEGER, SYMTAB_IDTYPE_VARIABLE, 0);

  input = "\
    if 3 + 4 < 10 then\
      x:= 10\
    else\
      x:= 1\
";

  sourcecode = fmemopen (input, strlen(input), "r");
  target = fopen("source.out", "w");

  lookahead = gettoken(sourcecode);
  ifstmt();
  
  test_clearenv();
  ok1("if stmt case insensitive");

  return exit_status();
}


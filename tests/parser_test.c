#include <stdio.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "../src/parser.h"
#include "tap.h"
#include "../src/debug.h"

FILE *input;
FILE *sourcecode;
FILE *target;

void test_clearenv() 
{
  current_line = 1;
  lextape_nextentry = 0;
  symtab_nextentry = 1;

  fclose(target);
  fclose(input);

  debug_clear();
}

void fake_declaration(char const *identifier, int dtype, int idtype, int offset)
{
  symtab_insert(identifier, dtype, idtype, offset);
}

int main(int argc, char **argv)
{
  plan_tests(11);

  debug_init();

  int token;
  char *inputcode;

  /* Simple program with declarations of variables only */
  inputcode = "\
    PROGRAM seila;\
\
    VAR \
      a:INTEGER;\
      b,c,d:REAL;\
\
    BEGIN\
    END.\
";

  target = fopen("source.out", "w");
  input = fmemopen(inputcode, strlen(inputcode), "r");
  sourcecode = (FILE *) debug_change_sourcecode(input);
  debug_send_sourcecode(sourcecode);

  lookahead = gettoken(sourcecode);
  program();

  test_clearenv();
  ok1("Simple program with declarations of variables only");

  /*Complete procedure declaration*/
  inputcode = "\
    PROCEDURE proc;\
\
    VAR \
      a:INTEGER;\
      b,c,d:REAL;\
\
    BEGIN\
    END;\
";

  target = fopen("source.out", "w");
  input = fmemopen(inputcode, strlen(inputcode), "r");
  sourcecode = (FILE *) debug_change_sourcecode(input);
  debug_send_sourcecode(sourcecode);

  lookahead = gettoken(sourcecode);
  procedure();
  
  test_clearenv();
  ok1("Complete procedure declaration");

  /*Creating functions and procedures (module)*/
  inputcode = "\
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


  target = fopen("source.out", "w");
  input = fmemopen(inputcode, strlen(inputcode), "r");
  sourcecode = (FILE *) debug_change_sourcecode(input);
  debug_send_sourcecode(sourcecode);

  lookahead = gettoken(sourcecode);
  modules();
  
  test_clearenv();
  ok1("Creating functions and procedures (module)");

  /* Cleaning variables of a module */
  inputcode = "\
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

  target = fopen("source.out", "w");
  input = fmemopen(inputcode, strlen(inputcode), "r");
  sourcecode = (FILE *) debug_change_sourcecode(input);
  debug_send_sourcecode(sourcecode);

  lookahead = gettoken(sourcecode);
  modules();
  
  /* variavel local da funcao nao pode ser acessada mais*/
  ok1(!symtab_lookup("X"));

  test_clearenv();
  ok1("Creating functions and procedures (module)");

  /*Assign expressions*/
  /* add x to symtab for text -> fake declaration */
  fake_declaration("X", INTEGER, SYMTAB_IDTYPE_VARIABLE, 0);
  inputcode = "x := 3 + 4;";

  target = fopen("source.out", "w");
  input = fmemopen(inputcode, strlen(inputcode), "r");
  sourcecode = (FILE *) debug_change_sourcecode(input);
  debug_send_sourcecode(sourcecode);

  lookahead = gettoken(sourcecode);
  stmt();
  
  test_clearenv();
  ok1("Assign expressions");

  /*Multiple assign expressions*/
  fake_declaration("X", INTEGER, SYMTAB_IDTYPE_VARIABLE, 0);
  fake_declaration("Y", INTEGER, SYMTAB_IDTYPE_VARIABLE, 0);
  inputcode = "\
    x := 3 + 4;\
    y := x + 4\
";

  target = fopen("source.out", "w");
  input = fmemopen(inputcode, strlen(inputcode), "r");
  sourcecode = (FILE *) debug_change_sourcecode(input);
  debug_send_sourcecode(sourcecode);

  lookahead = gettoken(sourcecode);
  stmtlist();
  
  test_clearenv();
  ok1("Multiple assign expressions");

  /*Boolean expression*/
  inputcode = "3 + 4 < 10";

  target = fopen("source.out", "w");
  input = fmemopen(inputcode, strlen(inputcode), "r");
  sourcecode = (FILE *) debug_change_sourcecode(input);
  debug_send_sourcecode(sourcecode);

  lookahead = gettoken(sourcecode);
  expression();
  
  test_clearenv();
  ok1("Boolean expression");

  /*if stmt*/
  fake_declaration("X", INTEGER, SYMTAB_IDTYPE_VARIABLE, 0);

  inputcode = "\
    IF 3 + 4 < 10 THEN\
      x:= 10\
    ELSE\
      x:= 1\
";

  target = fopen("source.out", "w");
  input = fmemopen(inputcode, strlen(inputcode), "r");
  sourcecode = (FILE *) debug_change_sourcecode(input);
  debug_send_sourcecode(sourcecode);

  lookahead = gettoken(sourcecode);
  ifstmt();
  
  test_clearenv();
  ok1("if stmt");

  /*if stmt case insensitive*/
  fake_declaration("X", INTEGER, SYMTAB_IDTYPE_VARIABLE, 0);

  inputcode = "\
    if 3 + 4 < 10 then\
      x:= 10\
    else\
      x:= 1\
";

  target = fopen("source.out", "w");
  input = fmemopen(inputcode, strlen(inputcode), "r");
  sourcecode = (FILE *) debug_change_sourcecode(input);
  debug_send_sourcecode(sourcecode);

  lookahead = gettoken(sourcecode);
  ifstmt();
  
  test_clearenv();
  ok1("if stmt case insensitive");

  /*call builtin function*/
  insert_builtins_in_symtab();
  fake_declaration("X", INTEGER, SYMTAB_IDTYPE_VARIABLE, 0);

  inputcode = "\
    write(x); \
";

  target = fopen("source.out", "w");
  input = fmemopen(inputcode, strlen(inputcode), "r");
  sourcecode = (FILE *) debug_change_sourcecode(input);
  debug_send_sourcecode(sourcecode);

  lookahead = gettoken(sourcecode);
  factor();
  
  test_clearenv();
  ok1("call builtin function");

  debug_finalize();
  return exit_status();
}


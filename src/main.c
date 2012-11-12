#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin_functions.h"
#include "parser.h"
#include "gencode.h"

FILE *sourcecode;
FILE *target;

/**
 * Está função tem como objetivo montar e linkar o código
 * gerado durante a tradução.
 */
void asssembly_and_link(void)
{
  char *arguments[4];

  pid_t pid = fork();
  if(pid >= 0) {
    if(pid == 0) {
      arguments[0] = "/usr/bin/as";
      arguments[1] = "source.out";
      arguments[2] = "-o";
      arguments[3] = "source.o";
    } else {
    }


    execv("/usr/bin/as", arguments);

  } else {
    fprintf(stderr, "Assembly failed!\n");
  }

  wait();

  pid = fork();
  if(!pid >= 0) {
    arguments[0] = "/usr/bin/ld";
    arguments[1] = "source.o";
    arguments[2] = "-o";
    arguments[3] = "source";

    execv("/usr/bin/ld", arguments);

  } else {
    fprintf(stderr, "Linker failed!\n");
  }
}

int main(int argc, char **argv) 
{

  /*** Debug: inicializa ambiente */
  debug_init();
  /***  */

  target = fopen("source.out", "w");

  /*** Debug: copia entrada em uma stream manipulavel */
  sourcecode = debug_change_sourcecode(stdin);
  /*** */

  /*** Debug: envia codigo para a janela de debug */
  debug_send_sourcecode(sourcecode);
  /*** */

  insert_builtins_in_symtab();

  lookahead = gettoken(sourcecode);
  program();

  /** */
  fclose(target);
  /** */

  /*** Debug: finaliza ambiente */
  debug_finalize();
  /*** */

  /** */
  asssembly_and_link();
  /** */

  return 0;
}


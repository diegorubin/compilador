#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

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
  int status = 0;
  char *arguments[4];
  pid_t pidas, pidld;
  
  pidas = fork();
  if(pidas >= 0) {
    if(pidas == 0) {
      printf("Montando...\n");

      arguments[0] = "/usr/bin/as";
      arguments[1] = "source.s";
      arguments[2] = "-o";
      arguments[3] = "source.o";

      execvp("/usr/bin/as", arguments);
      
      exit(0);
    } else {

    }

  } else {
    fprintf(stderr, "Assembly failed!\n");
  }

  waitpid(pidas,&status,0);

  pidld = fork();
  if(pidld >= 0) {

    if(pidld == 0) {
      printf("Linkando...\n");

      arguments[0] = "/usr/bin/ld";
      arguments[1] = "source.o";
      arguments[2] = "-o";
      arguments[3] = "source";

      execvp("/usr/bin/ld", arguments);

      exit(0);
    } else {
    }

  } else {
    fprintf(stderr, "Linker failed!\n");
  }

  waitpid(pidld,&status,0);

}

int main(int argc, char **argv) 
{

  sourcecode = stdin;
  target = fopen("source.s", "w");

  /**
   * Neste ponto as funções previamente criadas, serão
   * adicionadas na tabela de símbolos.
   */
  insert_builtins_in_symtab();
  /** */

  lookahead = gettoken(sourcecode);
  program();

  /** */
  fclose(target);
  /** */

  /** */
  asssembly_and_link();
  /** */

  return 0;
}


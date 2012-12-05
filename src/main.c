#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <builtin_functions.h>
#include <parser.h>
#include <gencode.h>

FILE *sourcecode;
FILE *target;

/**
 * Está função tem como objetivo montar e linkar o código
 * gerado durante a tradução.
 */
void asssembly_and_link(const char *out)
{
  int status = 0;
  char *arguments[4];
  pid_t pidas, pidld;
  
  pidas = fork();
  if(pidas >= 0) {
    if(pidas == 0) {
      printf("Montando...\n");

      arguments[0] = "/usr/bin/as";
      sprintf(arguments[1], "%s.s", out);
      printf("arg: %s", arguments[1]);
      arguments[2] = "-o";
      sprintf(arguments[3], "%s.o", out);

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
      sprintf(arguments[1], "%s.o", out);
      arguments[2] = "-o";
      sprintf(arguments[3], "%s", out);

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

  /**
   * Não foi informado os parametros minimos.
   */
  if(argc < 3) {
    printf("Não foi informado os parametros minimos\n");
    printf("Uso: ./mypas <codigo_fonte> <destino>\n");
    return -1;
  }

  sourcecode = fopen(argv[1], "r");
  target = fopen(argv[2], "w");

  /**
   * Inserindo funções previamente criadas
   */
  insert_builtins_in_symtab();
  /** */

  lookahead = gettoken(sourcecode);
  program();

  /** */
  fclose(target);
  fclose(sourcecode);
  /** */

  return 0;
}


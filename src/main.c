#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

FILE *sourcecode;
FILE *target;

int main(int argc, char **argv) {

  /**
   * Variavel para controle de erro.
   * Salva a linha do codigo fonte
   * em que algum erro foi encontrado.
   */
  current_line = 1;

  sourcecode = stdin;
  target = open("source.out", "w");

  lookahead = gettoken(sourcecode);
  program();

  return 0;
}


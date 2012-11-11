#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "gencode.h"

FILE *sourcecode;
FILE *target;

int main(int argc, char **argv) {

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

  lookahead = gettoken(sourcecode);
  program();

  /*** Debug: finaliza ambiente */
  debug_finalize();
  /*** */

  return 0;
}


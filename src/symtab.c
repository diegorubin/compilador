#include "symtab.h"

/*
 * Colunas da tabela de simbolos.
 *
 * 0: Endereco na fita de identificadores
 * 1: Tipo de dado do identificador
 * 2: Tipo do identificador {1 => Variable, 
 *                           3 => Procedure,
 *                           4 => Function}
 *
 */

int symtab[MAXSYMTABENTRIES][4];
symtab_nexentry = 1;

char lextape[MAXLEXTAPESIZE];
lextape_nextentry = 0;

int symtab_insert(char const *symbol, int dtype, int idtype, int offset)
{
  symtab[symtab_nexentry][0] = lextape_nextentry;
  symtab[symtab_nexentry][1] = dtype;
  symtab[symtab_nexentry][2] = idtype;

  strcpy(lextape + lextape_nextentry, symbol);
  lextape_nextentry += strlen(symbol) + 1;

  /* return position which was inserted */
  return symtab_nexentry++;
}

int symtab_lookup(char const *symbol)
{
  int i;
  for(i = symtab_nexentry - 1; i > 0; i--) {
    if(strcmp(lextape+symtab[i][0], symbol) == 0) return i;
  }
  return 0;
}


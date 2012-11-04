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
symtab_nextentry = 1;

char lextape[MAXLEXTAPESIZE];
lextape_nextentry = 0;

int symtab_insert(char const *symbol, int dtype, int idtype, int offset)
{
  symtab[symtab_nextentry][SYMTAB_COL_IDENTIFIER] = lextape_nextentry;
  symtab[symtab_nextentry][SYMTAB_COL_DATA_TYPE] = dtype;
  symtab[symtab_nextentry][SYMTAB_COL_IDENTIFIER_TYPE] = idtype;
  symtab[symtab_nextentry][SYMTAB_COL_OFFSET] = offset;

  strcpy(lextape + lextape_nextentry, symbol);
  lextape_nextentry += strlen(symbol) + 1;

  /* return position which was inserted */
  return symtab_nextentry++;
}

int symtab_lookup(char const *symbol)
{
  int i;
  for(i = symtab_nextentry - 1; i > 0; i--) {
    if(strcmp(lextape+symtab[i][SYMTAB_COL_IDENTIFIER], symbol) == 0) return i;
  }
  return 0;
}


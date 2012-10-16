#include "symtab.h"

int symtab[MAXSYMTABENTRIES][4];
int symtab_nexentry = 1;

char lextape[MAXLEXTAPESIZE];
int lextape_nextentry = 0;

int symtab_insert(char const *symbol, int dtype)
{
  symtab[symtab_nexentry][0] = lextape_nextentry;
  symtab[symtab_nexentry][1] = dtype;

  strcpy(lextape + lextape_nextentry, symbol);
  lextape_nextentry += strlen(symbol) + 1;

  /* return position which was inserted */
  return symtab_nexentry++;
}

int symtab_lookup(char const *symbol)
{
  int i;
  for(i = symtab_nexentry - 1; i > 0; i--) {
    /**XXX: RETIRAR
     * Por isso os simbolos sao separados por 0,
     * pois ele delimita uma string.
     */
    if(strcmp(lextape+symtab[i][0], symbol) == 0) return i;
  }
  return 0;
}


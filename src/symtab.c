#include "symtab.h"

/*
 * Colunas da tabela de simbolos.
 *
 * 0: Endereco na fita de identificadores
 * 1: Tipo de dado do identificador
 * 2: Tipo do identificador {1 => Variable, 
 *                           3 => Procedure,
 *                           4 => Function,
 *                           5 => Parameter}
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

  /* Debug: 
   * Imprimindo symtab no debugger
   */
  symtab_print();
  /** */
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

void symtab_dispose_local_variables(void)
{
  while(symtab[--symtab_nextentry][SYMTAB_COL_IDENTIFIER_TYPE] != SYMTAB_IDTYPE_FUNCTION &&
        symtab[symtab_nextentry][SYMTAB_COL_IDENTIFIER_TYPE] != SYMTAB_IDTYPE_PROCEDURE);
  symtab_nextentry++;
}

/*
 * Esta função foi criada para auxiliar na declaracao
 * de procedimentos/funcoes e sua parametros.
 * Nós só temos acesso ao tipo da função após a declaração
 * dos parametros. Portando poderemos utilizar está
 * função para atualizar o tipo.
 */
void symtab_update_dtype(int pos, int dtype)
{
  symtab[pos][SYMTAB_COL_DATA_TYPE] = dtype;
}

/* funcoes para log */
/* escreve em um arquivo o tabela de simbolos e o lextape */
void symtab_print(void)
{
  char message[2048];
  int i;

  if(debug_isinitialized() > 0 ) {

    sprintf(message,
            "%d\n",
            DEBUG_SYMTABCLEAR);
    debug_send_message(message);

    for(i = symtab_nextentry - 1; i > 0; i--) {
      sprintf(message,
              "%d\n",
              DEBUG_SYMTABINSERT);
      debug_send_message(message);

      sprintf(message,
              "%s,%d,%d,%d\n",
              lextape+symtab[i][SYMTAB_COL_IDENTIFIER],
              symtab[i][SYMTAB_COL_DATA_TYPE],
              symtab[i][SYMTAB_COL_IDENTIFIER_TYPE],
              symtab[i][SYMTAB_COL_OFFSET]); 

      debug_send_message(message);
    }
    sprintf(message,
            "%d\n",
            DEBUG_WAITING);
    debug_send_message(message);

  }
}


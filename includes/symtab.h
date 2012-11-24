#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <debug.h>
#include <keywords.h>
#include <tokens.h>

#define MAXSYMTABENTRIES 0x100000
#define MAXLEXTAPESIZE 0x1000000

/**
 * Constantes que descrevem cada coluna da 
 * tabela de simbolos.
 */
enum {
  SYMTAB_COL_IDENTIFIER=0,
  SYMTAB_COL_DATA_TYPE,
  SYMTAB_COL_IDENTIFIER_TYPE,
  SYMTAB_COL_OFFSET,
  SYMTAB_COL_NPARAMS,
};

/**
 * Constantes utilizadas na coluna tipo de variavel
 */
enum {
  SYMTAB_IDTYPE_VARIABLE=1,
  SYMTAB_IDTYPE_CONSTANT,
  SYMTAB_IDTYPE_PROCEDURE,
  SYMTAB_IDTYPE_FUNCTION,
  SYMTAB_IDTYPE_PARAMETER,
};

/* 
 * Tabela de simbolos onde serão armazenados
 * todoas as variaveis, constantes, procedimentos e
 * funções.
 */
int symtab[MAXSYMTABENTRIES][5];

/*
 * O objetivo desta segunda tabela é armazenar as
 * informações dos parametros das funções e 
 * procedimentos criados.
 */
int symtab_params[MAXSYMTABENTRIES][2];

int symbol_entry;

char lextape[MAXLEXTAPESIZE];
int lextape_nextentry;
int symtab_nextentry;
int symtab_param_nextentry;

/* funções para a tabela de simbolos */
int symtab_insert(char const *identifier, int dtype, int idtype, int offset);
int symtab_lookup(char const *identifier);
void symtab_update_dtype(int pos, int dtype);
void symtab_update_nparams(int pos, int nparams);

/* funções para a tabela de parametros */
int symtab_param_insert(int idpos, int dtype);
int symtab_param_lookup(int idpos, int parampos);

void symtab_dispose_local_variables(void);
void symtab_print(void);

#endif /* __SYMTAB_H__*/


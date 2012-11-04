#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include "keywords.h"
#include "tokens.h"

#define MAXSYMTABENTRIES 0x100000
#define MAXLEXTAPESIZE 0x1000000

enum {
  SYMTAB_COL_IDENTIFIER=0,
  SYMTAB_COL_DATA_TYPE,
  SYMTAB_COL_IDENTIFIER_TYPE,
  SYMTAB_COL_OFFSET,
};

enum {
  SYMTAB_IDTYPE_VARIABLE=1,
  SYMTAB_IDTYPE_CONSTANT,
  SYMTAB_IDTYPE_PROCEDURE,
  SYMTAB_IDTYPE_FUNCTION,
};

int symtab[MAXSYMTABENTRIES][4];
int symbol_entry;

char lextape[MAXLEXTAPESIZE];
int lextape_nextentry;
int symtab_nextentry;

int symtab_insert(char const *identifier, int dtype, int idtype, int offset);
int symtab_lookup(char const *identifier);

#endif /* __SYMTAB_H__*/


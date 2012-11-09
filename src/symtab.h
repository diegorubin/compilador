#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
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
  SYMTAB_IDTYPE_PARAMETER,
};

int symtab[MAXSYMTABENTRIES][4];
int symbol_entry;

char lextape[MAXLEXTAPESIZE];
int lextape_nextentry;
int symtab_nextentry;

int symtab_insert(char const *identifier, int dtype, int idtype, int offset);
int symtab_lookup(char const *identifier);

void symtab_dispose_local_variables(void);
void symtab_print(void);

#endif /* __SYMTAB_H__*/


#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include "keywords.h"
#include "tokens.h"

#define MAXSYMTABENTRIES 0x100000
#define MAXLEXTAPESIZE 0x1000000

int symtab[MAXSYMTABENTRIES][4];
int symbol_entry;

char lextape[MAXLEXTAPESIZE];
int lextape_nextentry;

int symtab_insert(char const *identifier, int dtype, int idtype, int offset);
int symtab_lookup(char const *identifier);

#endif /* __SYMTAB_H__*/


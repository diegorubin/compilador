#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#define MAXSYMTABENTRIES 0x100000
#define MAXLEXTAPESIZE 0x1000000

int symtab_insert(char const *identifier, int type);
int symtab_lookup(char const *identifier);

#endif /* __SYMTAB_H__*/


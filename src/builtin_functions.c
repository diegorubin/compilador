#include "builtin_functions.h"

char *builtins[] = {
  "WRITE",
  "READ",
};

void builtin_write(FILE *target) 
{
}

void builtin_read(FILE *target)
{
}

void insert_builtins_in_symtab(void)
{
  int i;
  int offset = 0;
  for(i = WRITE; i <= READ; i++){
    symtab_insert(builtins[i-WRITE], 0, SYMTAB_IDTYPE_PROCEDURE, offset);
    offset += 4;
  }
}

int isbuiltin(char const *identifier)
{
  int i;
  for(i = WRITE; i <= READ; i++){
    if(strcmp(builtins[i-WRITE], identifier) == 0) return i;
  }
  return 0;
}


#include "builtin_functions.h"

char *builtins[] = {
  "WRITE",
  "READ",
};

void builtin_write(FILE *target) 
{
  fprintf(target, "_write:\n");
  fprintf(target, "\tmovl $4, %%eax\n");
  fprintf(target, "\tmovl $1, %%ebx\n");
  fprintf(target, "\tmovl %%ebp, %%ebx\n"); /*TODO: checar local do parametro*/
  fprintf(target, "\tmovl $12, %%edx\n");
  fprintf(target, "\tint $0x80\n");
  fprintf(target, "\tret\n");
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

/*
 * Função para escrever os procedimentos criados no arquivo alvo.
 */
int builtin_write_functions(FILE *target)
{
  builtin_write(target);
}


#include "builtin_functions.h"

char *builtins[] = {
  "WRITE",
  "READ",
};

void builtin_write(FILE *target) 
{
  fprintf(target, "_WRITE:\n");

  /* inicializando funcao */
  fprintf(target, "\tpush %%ebp\n");
  fprintf(target, "\tmovl %%esp, %%ebp\n");

  fprintf(target, "\tmovl $4, %%eax\n");
  fprintf(target, "\tmovl $4, %%eax\n");
  fprintf(target, "\tmovl $1, %%ebx\n");

  /* mover string para ecx */
  fprintf(target, "\tmovl $5, %%ecx\n");

  /* tamanho da string */
  fprintf(target, "\tmovl $12, %%edx\n");

  fprintf(target, "\tint $0x80\n");

  /* fechando funcao */
  fprintf(target, "\tmovl %%ebp, %%esp\n");
  fprintf(target, "\tpop %%ebp\n");
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
    /*TODO: Adicionar parametros na tabela de parametros*/
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

/**
 * Função para escrever os procedimentos criados no arquivo alvo.
 */
int builtin_write_functions(FILE *target)
{
  builtin_write(target);
}


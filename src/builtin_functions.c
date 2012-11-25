#include "builtin_functions.h"

char *builtins[] = {
  "WRITE",
  "READ",
};

void builtin_write(FILE *target) 
{
  fprintf(target, "_WRITE:\n");

  /* inicializando funcao */
  fprintf(target, "\tpushl %%ebp\n");
  fprintf(target, "\tmovl %%esp, %%ebp\n");

  /* value uint to ascii */
  fprintf(target, "\tmovl 8(%%ebp), %%eax\n");
  fprintf(target, "\taddl $48, %%eax\n");
  fprintf(target, "\taddl %%eax, 8(%%ebp)\n");

  /* preparando syscall */
  fprintf(target, "\tmovl $4, %%eax\n");
  /* file description : screen*/
  fprintf(target, "\tmovl $1, %%ebx\n");

  /* mover string para ecx */
  fprintf(target, "\tmovl 8(%%ebp), %%ecx\n");

  /* tamanho da string */
  fprintf(target, "\tmovl $1, %%edx\n");

  /* syscall */
  fprintf(target, "\tint $0x80\n");

  /* nova linha */
  fprintf(target, "\n\t#imprimindo nova linha\n");
  fprintf(target, "\tmovl $4, %%eax\n");
  fprintf(target, "\tmovl $1, %%ebx\n");
  fprintf(target, "\tmovl $newline, %%ecx\n");
  fprintf(target, "\tmovl $1, %%edx\n");
  fprintf(target, "\tint $0x80\n");

  /* fechando funcao */
  fprintf(target, "\n\t#encerrando funcao\n");
  fprintf(target, "\tmovl %%ebp, %%esp\n");
  fprintf(target, "\tpopl %%ebp\n");
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
    symtab_param_insert(i, INTEGER);
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


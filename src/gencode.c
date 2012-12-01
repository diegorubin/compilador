#include "gencode.h"

int label_count = 0;

/**
 * Imprime no arquivo de saida a tradução do comando program.
 */
void gencode_set_main_entry_point(const char *symbol)
{
  fprintf(target, ".data\n newline: .string \"\\n\"\n");
  fprintf(target, "\t.section .text\n");
  fprintf(target, ".globl _%s\n", symbol);
  fprintf(target, "\t.type main, @function\n");
  fprintf(target, "_%s:\n", symbol);
}

/**
 * Imprime no arquivo de saida a declaração da seção bss
 */
void gencode_bsssection(void)
{
  fprintf(target,"\t.section .bss\n");
}

/**
 * Gera o código inicial de um procedimento
 */
void gencode_procedure_start(const char *symbol)
{
  fprintf(target,"\n\t#inicio de um procedimento\n\n");

  gencode_start_label(symbol);
  fprintf(target,"\t.type _%s,@function\n", symbol);

  fprintf(target,"\tpushl %%ebp\n");
  fprintf(target,"\tmovl %%esp, %%ebp\n");
}

/**
 * Gera o código final de um procedimento
 */
void gencode_procedure_end(void)
{
  fprintf(target,"\tmovl  %%ebp, %%esp\n");
  fprintf(target,"\tpop  %%ebp\n");
  fprintf(target,"\tret\n");
}

/**
 * Gera o código inicial de uma funcao 
 */
void gencode_function_start(const char *symbol)
{
  fprintf(target,"\n\t#inicio de uma funcao\n\n");

  gencode_start_label(symbol);
  fprintf(target,"\t.type _%s,@function\n", symbol);

  fprintf(target,"\tpushl %%ebp\n");
  fprintf(target,"\tmovl %%esp, %%ebp\n");
}

/**
 * Gera o código final de uma funcao 
 */
void gencode_function_end(void)
{
  fprintf(target,"\tmovl  %%ebp, %%esp\n");
  fprintf(target,"\tpop  %%ebp\n");
  fprintf(target,"\tret\n");
}

/**
 * Imprime no arquivo de saida o inicio de um bloco de comandos,
 * tanto para funções e procedimentos como para o programa principal.
 */
void gencode_start_label(const char *symbol)
{
	fprintf(target,"_%s:\n", symbol);
}

/**
 * Imprime no arquivo de saida a chamada de sistema de fim de programa.
 */
void gencode_end_program(void)
{
  fprintf(target,"\t# encerrando programa\n");
	fprintf(target,"\tmovl $1, %%eax\n"); /* syscall do linux para sair */
	fprintf(target,"\tmovl $0, %%ebx\n"); /* status da saida do programa */
	fprintf(target,"\tint $0x80\n");
}

/**
 * Imprime no arquivo de saida a estrutura de chamada de função.
 */
void gencode_callfunction(const char *symbol)
{
	fprintf(target,"\tcall _%s\n", symbol);
}

/**
 * Imprime no arquivo de saida a estrutura de chamada de procedimento.
 */
void gencode_callprocedure(const char *symbol)
{
	fprintf(target,"\tcall _%s\n", symbol);
}

/** expression result stored in accumulator **/
/** push the accumulator onto the stack **/
void gencode_push_accumulator_onto_stack(void)
{
  fprintf(target,"\n\t#Enviando acumulador pra stack\n");
  fprintf(target,"\tpush %%eax\n");
}

void gencode_uint_move_to_accumulator(const char *uint)
{
  fprintf(target,"\n\t#Enviando inteiro para acumulador\n");
  fprintf(target,"\tmovl $%s,%%eax\n", uint);
}

int gencode_start_if_expression()
{
  char label[100];
  sprintf(label, "IF%d", ++label_count);

  fprintf(target,"\tcmpl (%%esp),%%eax\n");
  fprintf(target,"\tjz _%s\n", label);

  return label_count;
}

int gencode_start_else_expression(int labelif)
{
  char label[100];
  sprintf(label, "ELSE%d", ++label_count);

  fprintf(target, "\tjmp _%s\n", label);
  sprintf(label, "IF%d", labelif);
  gencode_start_label(label);

  return label_count;
}

int gencode_start_while()
{
  char label[100];
  sprintf(label, "WHILE_START%d", ++label_count);

  gencode_start_label(label);

  return label_count;
}

int gencode_start_do(int lblwhile)
{
  fprintf(target,"\tcmpl (%%esp),%%eax\n");
  fprintf(target, "\tjz _WHILE_END%d\n", lblwhile);
}

int gencode_end_while(int lblwhile)
{
  char label[100];
  sprintf(label, "WHILE_END%d", lblwhile);

  fprintf(target, "\tjmp _WHILE_START%d\n", lblwhile);
  gencode_start_label(label);
}


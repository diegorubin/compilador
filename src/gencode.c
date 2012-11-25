#include "gencode.h"

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
void gencode_procedure_start(void)
{
  fprintf(target,"\tpush %%ebp\n");
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
 * Imprime no arquivo de saida o inicio de um bloco de comandos,
 * tanto para funções e procedimentos como para o programa principal.
 */
void gencode_block(const char *symbol)
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


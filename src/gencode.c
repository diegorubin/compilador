#include "gencode.h"

/**
 * Imprime no arquivo de saida a tradução do comando program.
 */
void gencode_program(const char *symbol)
{
  fprintf(target, "\t.section .text\n");
  fprintf(target, "\t.globl _%s\n", symbol);
}

/**
 * Imprime no arquivo de saida a declaração da seção bss
 */
void gencode_bsssection(void)
{
	fprintf(target,"\t.section .bss\n");
}

/**
 * Imprime no arquivo de saida o inicio de um bloco de comandos,
 * tanto para funções e procedimentos como para o programa principal.
 */
void gencode_block(const char *symbol)
{
	fprintf(target,"\t_%s:\n", symbol);
}

/**
 * Imprime no arquivo de saida a chamada de sistema de fim de programa.
 */
void gencode_end_program(void)
{
	fprintf(target,"\tmovl $1, %%eax\n"); /* syscall do linux para sair */
	fprintf(target,"\tmovl $0, %%ebx\n"); /* status da saida do programa */
	fprintf(target,"\tint $0x80\n");
}


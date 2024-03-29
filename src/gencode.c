#include "gencode.h"

int label_count = 0;

/**
 * Imprime no arquivo de saída a tradução do comando program.
 */
void gencode_set_main_entry_point(const char *symbol)
{
  fprintf(target, ".data\n newline: .string \"\\n\"\n");
  fprintf(target, "\t.section .text\n");
  fprintf(target, ".global _%s\n", symbol);
  fprintf(target, "\t.type main, @function\n");
  fprintf(target, "_%s:\n", symbol);
}

/**
 * Imprime no arquivo de saída a declaração da seção bss
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
  fprintf(target,"\tpopl  %%ebp\n");
  fprintf(target,"\tret\n");
}

/**
 * Gera o código inicial de uma função 
 */
void gencode_function_start(const char *symbol)
{
  fprintf(target,"\n\t#inicio de uma função\n\n");

  gencode_start_label(symbol);
  fprintf(target,"\t.type _%s,@function\n", symbol);

  fprintf(target,"\tpushl %%ebp\n");
  fprintf(target,"\tmovl %%esp, %%ebp\n");
}

/**
 * Gera o código final de uma função 
 */
void gencode_function_end(void)
{
  fprintf(target,"\tmovl  %%ebp, %%esp\n");
  fprintf(target,"\tpopl  %%ebp\n");
  fprintf(target,"\tret\n");
}

/**
 * Imprime no arquivo de saída o inicio de um bloco de comandos,
 * tanto para funções e procedimentos como para o programa principal.
 */
void gencode_start_label(const char *symbol)
{
  fprintf(target,"_%s:\n", symbol);
}

/**
 * Imprime no arquivo de saída a chamada de sistema de fim de programa.
 */
void gencode_end_program(void)
{
  fprintf(target,"\t# encerrando programa\n");
  fprintf(target,"\tmovl $1, %%eax\n"); /* syscall do linux para sair */
  fprintf(target,"\tmovl $0, %%ebx\n"); /* status da saída do programa */
  fprintf(target,"\tint $0x80\n");
}

/**
 * Imprime no arquivo de saída a estrutura de chamada de função.
 */
void gencode_callfunction(const char *symbol)
{
  fprintf(target,"\tcall _%s\n", symbol);
}

/**
 * Imprime no arquivo de saída a estrutura de chamada de procedimento.
 */
void gencode_callprocedure(const char *symbol)
{
  fprintf(target,"\tcall _%s\n", symbol);
}

/**
 * Coloca valor do acumulador no topo da pilha.
 */
void gencode_push_accumulator_onto_stack(void)
{
  fprintf(target,"\n\t#Enviando acumulador pra stack\n");
  fprintf(target,"\tpushl %%eax\n");
}

/**
 * Coloca uma constante inteira sem sinal no topo da pilha.
 */
void gencode_uint_push(const char *uint)
{
  fprintf(target,"\n\t#Enviando inteiro para topo da pilha\n");
  fprintf(target,"\tmovl $%s,%%eax\n", uint);
  fprintf(target,"\tpushl %%eax\n");
}

/**
 * Coloca uma variável global no topo da pilha.
 */
void gencode_global_var_push(const char *var)
{
  fprintf(target,"\n\t#Enviando variável global para topo da pilha\n");
  fprintf(target,"\tmovl $%s,%%eax\n", var);
  fprintf(target,"\tpushl %%eax\n");
}

/**
 * Coloca uma variável local no topo da pilha.
 */
void gencode_local_var_push(int offset)
{
  fprintf(target,"\n\t#Enviando variável local para topo da pilha\n");
  fprintf(target,"\tmovl %d(%%ebp),%%eax\n", offset);
  fprintf(target,"\tpushl %%eax\n");
}

/**
 * Inicio do código de uma expressão if.
 */
int gencode_start_if_expression()
{
  char label[100];
  sprintf(label, "IF%d", ++label_count);

  fprintf(target,"\tjz _%s\n", label);

  return label_count;
}

/**
 * Inicio do código de uma expressão else.
 */
int gencode_start_else_expression(int labelif)
{
  char label[100];
  sprintf(label, "ELSE%d", ++label_count);

  fprintf(target, "\tjmp _%s\n", label);
  sprintf(label, "IF%d", labelif);
  gencode_start_label(label);

  return label_count;
}

/**
 * Inicio do código de uma expressão while.
 */
int gencode_start_while()
{
  char label[100];
  sprintf(label, "WHILE_START%d", ++label_count);

  gencode_start_label(label);

  return label_count;
}

/**
 * Após avaliação da expressão passado ao comando while, checa se é
 * necessário pular para o final. Está função foi criada separadamente da
 * tradução do inicio da expressão while, pois o resultado da expressão deve
 * ser avaliado todas as iterações.
 */
void gencode_start_do(int lblwhile)
{
  fprintf(target, "\tjz _WHILE_END%d\n", lblwhile);
}

/**
 * Encerra uma expressão while.
 */
void gencode_end_while(int lblwhile)
{
  char label[100];
  sprintf(label, "WHILE_END%d", lblwhile);

  fprintf(target, "\tjmp _WHILE_START%d\n", lblwhile);
  gencode_start_label(label);
}

/**
 * Inicia o laço utilizando o comando repeat.
 */
int gencode_start_repeat()
{
  char label[100];
  sprintf(label, "REPEAT_START%d", ++label_count);

  gencode_start_label(label);

  return label_count;
}

/**
 * Finaliza o laço. Caso a expressão seja falsa um salto
 * até o inicio do laço é realizado.
 */
void gencode_end_repeat(int lblrepeat)
{
  fprintf(target, "\tjz _REPEAT_START%d\n", lblrepeat);
}

/**
 * Nega o conteudo que está no topo da pilha.
 */
void gencode_neg()
{
  fprintf(target,"\n\t#negando topo da pilha\n");
  fprintf(target,"\tpopl %%eax\n");
  fprintf(target,"\tneg %%eax\n");
  fprintf(target,"\tpushl %%eax\n");
}

/**
 * Soma, subtrai ou realiza ou lógico utilizando os dois últimos 
 * valores encontrados na pilha.
 */
void gencode_execute_add(int op)
{
  switch(op) {
    case '+':
      fprintf(target,"\n\t#realizando soma\n");
      fprintf(target,"\tpopl %%eax\n");
      fprintf(target,"\tpopl %%ebx\n");
      fprintf(target,"\taddl %%ebx, %%eax\n");
      fprintf(target,"\tpushl %%eax\n");
      break;
    case '-':
      fprintf(target,"\n\t#relizando subtração\n");
      fprintf(target,"\tpopl %%eax\n");
      fprintf(target,"\tpopl %%ebx\n");
      fprintf(target,"\tsubl %%ebx, %%eax\n");
      fprintf(target,"\tpushl %%eax\n");
      break;
    case OR:
      fprintf(target,"\n\t#relizando ou\n");
      fprintf(target,"\tpopl %%eax\n");
      fprintf(target,"\tpopl %%ebx\n");
      fprintf(target,"\torl %%ebx, %%eax\n");
      fprintf(target,"\tpushl %%eax\n");
      break;
  }
}

/**
 * Multiplicação, divisão ou realiza e lógico utilizando os dois últimos 
 * valores encontrados na pilha.
 */
void gencode_execute_mul(int op)
{
  switch(op) {
    case '*':
      fprintf(target,"\n\t#relizando multiplicação\n");
      fprintf(target,"\n\t popl %%eax");
      fprintf(target,"\n\t popl %%ebx");
      fprintf(target,"\n\t imul %%ebx");
      fprintf(target,"\n\t pushl %%ebx");
      break;
    case '/':
      fprintf(target,"\n\t#relizando divisão\n");
      fprintf(target,"\t popl %%ecx");
      fprintf(target,"\t popl %%eax");  
      fprintf(target,"\t cltd");
      fprintf(target,"\t idivl %%ecx");  
      break;
    case AND:
      fprintf(target,"\n\t#relizando and\n");
      fprintf(target,"\tpopl %%eax\n");
      fprintf(target,"\tpopl %%ebx\n");
      fprintf(target,"\tandl %%ebx, %%eax\n");
      fprintf(target,"\tpushl %%eax\n");
      break;
  }
}

/**
 * Tradução de expressões relacionais.
 */
void gencode_execute_relational(int op)
{
  fprintf(target,"\n\t#Realizand operação relacional.\n");
  fprintf(target,"\tpopl %%eax\n");
  fprintf(target,"\tpopl %%ebx\n");
  fprintf(target,"\tcmpl %%eax,%%ebx\n");
  switch(op){
    case NEQ:
      fprintf(target,"\tsetne %%al\n");
      break;
    case LEQ:
      fprintf(target,"\tsetle %%al\n");
      break;
    case '<':
      fprintf(target,"\tsetl %%al\n");
      break;
    case GEQ:
      fprintf(target,"\tsetge %%al\n");
      break;
    case '>':
      fprintf(target,"\tsetg %%al\n");
      break;
    case '=':
      fprintf(target,"\tsete %%al\n");
      break;
  }

}

/**
 * Move topo da pilha para uma variável global.
 */
void gencode_global_assgnmt(const char *var)
{
  fprintf(target, "\n\t#atribuição de variável global\n");
  fprintf(target,"\tmovl %%eax, %s\n", var);
}

/**
 * Move topo da pilha para uma variável local.
 */
void gencode_local_assgnmt(int offset)
{
  fprintf(target, "\n\t#atribuição de variável local\n");
  fprintf(target,"\tmovl %%eax, %d(%%ebp)\n", offset);
}

/**
 * Traduz declaração de uma variável global.
 */
void gencode_declare_global_var(const char *symbol, int len)
{
  fprintf(target,"\t.common %s,%d,%d\n",symbol,len,len);
}


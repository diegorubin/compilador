#ifndef __GENCODE__H__
#define __GENCODE__H__

#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>

extern FILE *target;

void gencode_set_main_entry_point(const char *symbol);
void gencode_bsssection(void);
void gencode_block(const char *symbol);
void gencode_end_program(void);

void gencode_start_label(const char *symbol);

void gencode_procedure_start(const char *symbol);
void gencode_procedure_end(void);
void gencode_function_start(const char *symbol);
void gencode_function_end(void);

void gencode_callfunction(const char *symbol);
void gencode_callprocedure(const char *symbol);

void gencode_push_accumulator_onto_stack(void);
void gencode_uint_push(const char *uint);

int gencode_start_if_expression();
int gencode_start_else_expression(int labelif);

int gencode_start_while();
void gencode_start_do(int lblwhile);
void gencode_end_while(int lblwhile);

int gencode_start_repeat();
void gencode_end_repeat(int lblrepeat);

void gencode_neg();
void gencode_execute_add(int op);
void gencode_global_var_push(const char *var);
void gencode_local_var_push(int offset);

void gencode_declare_global_var(const char *symbol, int len);

#endif // __GENCODE__H__


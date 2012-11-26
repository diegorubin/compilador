#ifndef __GENCODE__H__
#define __GENCODE__H__

#include <stdio.h>
#include <stdlib.h>

extern FILE *target;

void gencode_set_main_entry_point(const char *symbol);
void gencode_bsssection(void);
void gencode_block(const char *symbol);
void gencode_end_program(void);

void gencode_procedure_start(void);
void gencode_procedure_end(void);

void gencode_callfunction(const char *symbol);
void gencode_callprocedure(const char *symbol);

void gencode_push_accumulator_onto_stack(void);
void gencode_uint_move_to_accumulator(const char *uint);

void gencode_start_if_expression();

#endif // __GENCODE__H__


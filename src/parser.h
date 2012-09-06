#ifndef __PARSER_H__
#define __PARSER_H__

#include "lexer.h"
#include "tokens.h"

int lookup(char *key);
double store(double val, char *key);

void Expr(void);
void Term(void);
void Factor(void);

#endif /* __PARSER_H__ */


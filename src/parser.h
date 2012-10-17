#ifndef __PARSER_H__
#define __PARSER_H__

#include "lexer.h"
#include "tokens.h"
#include "symtab.h"

#define MAXSYMLIST 0x1000

void program(void);
void block(void);
void declarations(void);
void modules(void);
void procedure(void);
void function(void);
void idlist(void);
void type(void);
void stmtblock(void);
void stmtlist(void);
void ifstmt(void);
void whilestmt(void);
void repstmt(void);
void stmt(void);
void idstmt(void);
void formalparm(void);

void exprlist(void);
void expressionlist(void);
void expression(void);
void expr(void);
void term(void);
void factor(void);

int isrelationalop(token_t token);
char symlist[MAXSYMLIST][IDSIZE];

int sympos;

#endif /* __PARSER_H__ */


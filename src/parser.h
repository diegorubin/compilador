#ifndef __PARSER_H__
#define __PARSER_H__

#include "lexer.h"
#include "tokens.h"
#include "keywords.h"

void program(void);
void block(void);
void declarations(void);
void modules(void);
void procedure(void);
void function(void);
void idlist(void);
void type(void);
void stmtblock(void);
void ifblock(void);
void whileblock(void);
void repblock(void);
void idblock(void);

void expr(void);
void expression(void);

int isrelationalop(token_t token);

#endif /* __PARSER_H__ */


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

#endif /* __PARSER_H__ */


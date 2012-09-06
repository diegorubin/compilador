#ifndef __LEXER_H__
#define __LEXER_H__

#include "tokens.h"

#define IDSIZE 32

int lookahead;
extern FILE *sourcecode;

char lexeme[IDSIZE];

token_t get_token(FILE *buffer);
void match(int expected);

#endif /*__LEXER_H__*/


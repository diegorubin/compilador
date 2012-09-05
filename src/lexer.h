#ifndef __LEXER_H__
#define __LEXER_H__

#include "tokens.h"

int lookahead;

extern FILE *sourcecode;

char lexeme[32];

token_t get_token(FILE *buffer);
void match(int expected);

#endif /*__LEXER_H__*/


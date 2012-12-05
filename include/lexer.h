#ifndef __LEXER_H__
#define __LEXER_H__

#include <keywords.h>
#include <tokens.h>

#define IDSIZE 32

int lookahead;
extern FILE *sourcecode;

char lexeme[IDSIZE];
int current_line;

token_t get_token(FILE *buffer);
void match(int expected);

/* funcoes de auxilio */
int lexeme_pointer;
void put_lexeme(char c);
char get_lexeme();
void finalize_lexeme();
void clear_lexeme();

#endif /*__LEXER_H__*/


#ifndef __LEXER_H__
#define __LEXER_H__

#include "tokens.h"


token_t get_token(FILE *buffer);
int match(token_t expected);

#endif /*__LEXER_H__*/


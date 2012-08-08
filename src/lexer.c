#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int isID(FILE *buffer)
{
  char head = getc(buffer);
  if (isalpha(head)) {
    while (isalnum(head = getc(buffer)));
    ungetc(head, buffer);
    return ID;
  }
  ungetc(head, buffer);
  return 0;
}

token_t gettoken(FILE *buffer)
{
  char head;
  int token;

  /* skip spaces */
  while(isspace(head = getc(buffer)));
  ungetc(head, buffer);

  if(token = isID(buffer)) return token;

  return getc(buffer);
}

int match(token_t expected)
{
  return 0;
}




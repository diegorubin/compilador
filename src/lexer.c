#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

#define isoctal(x) ( '0' <= (x) && (x) < '8' )
#define ishexa(x)  ( isdigit(x) || 'a' <= tolower(x) && tolower(x) <= 'f')

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

isNUM(FILE *buffer)
{
  char head = getc(buffer);
  if (isdigit(head)) {
    if (head == '0') {
      if (tolower(head = getc(buffer)) == 'x') {
        while (isdigit(head = getc(buffer)) || ishexa(head));
        ungetc(head, buffer);
        return HEXA;
      }
      if (isoctal(head)) {
        while (isoctal(head = getc(buffer)));
        ungetc(head, buffer);
        return OCTA;
      }
      return ZERO;
    }
    while (isdigit(head = getc(buffer)));
    ungetc(head, buffer);
    return NUM;
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
  if(token = isNUM(buffer)) return token;

  return getc(buffer);
}

int match(token_t expected)
{
  return 0;
}




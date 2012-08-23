#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

#define isoctal(x) ( '0' <= (x) && (x) < '8' )
#define ishexa(x)  ( isdigit(x) || 'a' <= tolower(x) && tolower(x) <= 'f')

int isOPLUS(FILE *buffer)
{
    int head = getc(buffer);
    switch (head) {
    case '+':
    case '-':
        return OPLUS;
    }
    ungetc(head,buffer);
    return 0;
}

int isOTIMES(FILE *buffer)
{
    int head = getc(buffer);
    switch (head) {
    case '*':
    case '/':
        return OTIMES;
    }
    ungetc(head,buffer);
    return 0;
}

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

int isNUM(FILE *buffer)
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

      ungetc(head, buffer);
      return ZERO;
    }
    while (isdigit(head = getc(buffer)));
    ungetc(head, buffer);
    return NUM;
  }
  ungetc(head, buffer);
  return 0;
}

int isexp(FILE *buffer) 
{
  char head = getc(buffer);
  if(tolower(head) == 'e') {
    /* guardaremos o 'e' caso precise ser devolvido e não
     * podemos afirmar se ele está em minusculo
     */
    char reale = head;
    head = getc(buffer);

    /* o sinal é opcional */
    if(head != '+' && head != '-')
      ungetc(head, buffer);

    if(isdigit(head = getc(buffer))) {
      while(isdigit(head = getc(buffer)));
      ungetc(head, buffer);

      return 1;
    }

    ungetc(head,buffer);
    ungetc(reale,buffer);

    return 0;
  }

  ungetc(head,buffer);
  return 0;
}

int isFLOAT(FILE *buffer)
{
  char head;
  int numtype = isNUM(buffer);
  if(numtype){

    if(numtype == OCTA || numtype == HEXA) return numtype;

    if((head = getc(buffer)) == '.') {
      if(isdigit(head = getc(buffer))) {
        while(isdigit(head = getc(buffer)));
        ungetc(head, buffer);

        isexp(buffer);

        return FLOAT;
      }
      ungetc(head, buffer);
      return 0;
    }

    if(isexp(buffer)) {
      return FLOAT;
    }

    return numtype;

  }

  if((head = getc(buffer)) == '.') {
    if(isdigit(head = getc(buffer))) {
      while(isdigit(head = getc(buffer)));
      ungetc(head, buffer);

      isexp(buffer);

      return FLOAT;
    }
    ungetc('.', buffer);
    return 0;
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

  //if(token = isNUM(buffer)) return token;
  if(token = isID(buffer)) return token;
  if(token = isOPLUS(buffer)) return token;
  if(token = isOTIMES(buffer)) return token;
  if(token = isFLOAT(buffer)) return token;

  return getc(buffer);
}

void match(int expected)
{
  if (lookahead == expected) {
      lookahead = gettoken(sourcecode);
  } else {
    fprintf(stderr,
            "token mismatch: found %d but %d expected\n",
            lookahead, expected);
    exit(-1);
  }
}


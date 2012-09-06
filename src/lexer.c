#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

#define isoctal(x) ( '0' <= (x) && (x) < '8' )
#define ishexa(x)  ( isdigit(x) || 'a' <= tolower(x) && tolower(x) <= 'f')

int p;

int isOTIMES(FILE *buffer)
{
  int head = getc(buffer);

  switch (head) {
  case '*':
  case '/':
    lexeme[0] = head;
    lexeme[1] = 0;
    return OTIMES;
  }
  ungetc(head,buffer);
  return 0;
}

int isID(FILE *buffer)
{
  char head = getc(buffer);
  if (isalpha(head)) {
    lexeme[p++] = head;

    while (isalnum(head = getc(buffer))) lexeme[p++] = head;
    ungetc(head, buffer);

    lexeme[p] = 0;
    return ID;
  }
  ungetc(head, buffer);
  return 0;
}

int isNUM(FILE *buffer)
{
  char head = getc(buffer);

  if (isdigit(head)) {
    lexeme[p++] = head;

    if (head == '0') {
      if (tolower(head = getc(buffer)) == 'x') {
        lexeme[p++] = head;

        while (isdigit(head = getc(buffer)) || ishexa(head)) lexeme[p++] = head;

        lexeme[p] = 0;
        ungetc(head, buffer);

        return HEXA;
      }
      if (isoctal(head)) {
        lexeme[p++] = head;

        while (isoctal(head = getc(buffer))) lexeme[p++] = head;

        lexeme[p] = 0;
        ungetc(head, buffer);

        return OCTA;
      }

      lexeme[p] = 0;
      ungetc(head, buffer);
      
      return NUM;
    }

    while (isdigit(head = getc(buffer))) lexeme[p++] = head;

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
    lexeme[p++] = head;
    /* guardaremos o 'e' caso precise ser devolvido e não
     * podemos afirmar se ele está em minusculo
     */
    char reale = head;

    head = getc(buffer);
    lexeme[p++] = head;

    /* o sinal é opcional */
    if(head != '+' && head != '-'){
      p--;
      ungetc(head, buffer);
    }

    if(isdigit(head = getc(buffer))) {
      lexeme[p++] = head;
      while(isdigit(head = getc(buffer))) lexeme[p++] = head;
      lexeme[p] = 0;
      ungetc(head, buffer);

      return 1;
    }

    ungetc(head,buffer);

    ungetc(reale,buffer); p--; /*tambem volta o e*/
    

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
      lexeme[p++] = head;

      if(isdigit(head = getc(buffer))) {
        lexeme[p++] = head;

        while(isdigit(head = getc(buffer))) lexeme[p++] = head;
        lexeme[p] = 0;
        ungetc(head, buffer);

        isexp(buffer);

        return FLOAT;
      }
      ungetc(head, buffer);
      return FLOAT;
    }

    if(isexp(buffer)) {
      return FLOAT;
    }

    ungetc(head, buffer);

    lexeme[p] = 0;
    return numtype;

  }

  if((head = getc(buffer)) == '.') {
    lexeme[p++] = head;
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

  p = 0;

  /* skip spaces */
  while(isspace(head = getc(buffer))) if(head == '\n') return head;
  ungetc(head, buffer);

  if(token = isID(buffer)) return token;
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


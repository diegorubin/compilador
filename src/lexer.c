#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

int isID(FILE *buffer)
{
  int p = 0;

  char head = getc(buffer);
  if (isalpha(head)) {
    lexeme[p++] = head;

    while (isalnum(head = getc(buffer)) || head == '_')
      lexeme[p++] = head;
    ungetc(head, buffer);

    lexeme[p] = 0;
    return ID;
  }
  ungetc(head, buffer);
  return 0;
}

int isNUM(FILE *buffer)
{
  int p = 0;

  char head = getc(buffer);

  if (isdigit(head)) {
    lexeme[p++] = head;

    while (isdigit(head = getc(buffer)));
    ungetc(head, buffer);
    return UINT;
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

int isUINT(FILE *buffer)
{
  char head;
  int numtype = isNUM(buffer);
  if(numtype){

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
      return UINT;
    }

    return UINT;

  }

  if((head = getc(buffer)) == '.') {
    if(isdigit(head = getc(buffer))) {
      while(isdigit(head = getc(buffer)));
      ungetc(head, buffer);

      isexp(buffer);

      return UINT;
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

  /** 
   * skip spaces 
   * incrementa a contagem de linhas 
   */
  while(isspace(head = getc(buffer)))
    if(head == '\n') current_line++;
  ungetc(head, buffer);

  if(token = isUINT(buffer)) return token;
  if(token = isID(buffer)) {
    if(iskeyword(lexeme)) return iskeyword(lexeme);
    else return token;
  }

  return getc(buffer);
}

void match(int expected)
{
  if (lookahead == expected) {
      lookahead = gettoken(sourcecode);
  } else {
    fprintf(stderr,
            "in line %d \n"
            "token mismatch: found %d but %d expected\n",
            current_line, lookahead, expected);
    exit(-1);
  }
}


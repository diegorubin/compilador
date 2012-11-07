#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

int isID(FILE *buffer)
{
  char head = getc(buffer);
  if (isalpha(head)) {
    put_lexeme(head);

    while (isalnum(head = getc(buffer)) || head == '_')
      put_lexeme(head);
    
    ungetc(head, buffer);
    finalize_lexeme();

    return ID;
  }
  ungetc(head, buffer);
  return 0;
}

int isNUM(FILE *buffer)
{
  char head = getc(buffer);

  if (isdigit(head)) {
    put_lexeme(head);

    while (isdigit(head = getc(buffer))) put_lexeme(head);

    finalize_lexeme();
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

    put_lexeme(reale);
    head = getc(buffer);

    /* o sinal é opcional */
    if(head != '+' && head != '-')
      ungetc(head, buffer);

    if(isdigit(head = getc(buffer))) {
      put_lexeme(head);

      while(isdigit(head = getc(buffer))) put_lexeme(head);

      finalize_lexeme();
      ungetc(head, buffer);

      return 1;
    }

    get_lexeme();
    ungetc(head,buffer);
    
    finalize_lexeme();
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
      return FLOAT;
    }

    return UINT;

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

int isASSGNMT(FILE *buffer)
{
  char head;

  if((head = getc(buffer)) == ':') {
    if((head = getc(buffer)) == '=')
      return ASSGNMT;

    ungetc(head, buffer);
    ungetc(':', buffer);

    return 0;
  }

  ungetc(head, buffer);
  return 0;
}

token_t gettoken(FILE *buffer)
{
  char head;
  int token;

  clear_lexeme();

  /** 
   * skip spaces 
   * incrementa a contagem de linhas 
   */
  while(isspace(head = getc(buffer)))
    if(head == '\n') current_line++;
  ungetc(head, buffer);

  if(token = isUINT(buffer)) return token;
  if(token = isASSGNMT(buffer)) return token;
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
            "token mismatch: found %d but %d expected\n"
            "lexeme: %s\n",
            current_line, lookahead, expected, lexeme);
    exit(-1);
  }
}

void clear_lexeme() 
{
  lexeme_pointer = 0;
  lexeme[lexeme_pointer] = 0;
}

void put_lexeme(char c)
{
  if(islower(c)) c = towupper(c);
  lexeme[lexeme_pointer++] = c;
  lexeme[lexeme_pointer] = 0;
}

char get_lexeme()
{
  char c = lexeme[--lexeme_pointer];
  lexeme[lexeme_pointer] = 0;

  return c;
}

void finalize_lexeme()
{
  lexeme[lexeme_pointer] = 0;
}


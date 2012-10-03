#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

token_t gettoken(FILE *buffer)
{
  char head;
  int token;

  /* skip spaces */
  while(isspace(head = getc(buffer))) if(head == '\n') return head;
  ungetc(head, buffer);

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


#include "keywords.h"

char *keywords[] = {
  "BEGIN",
  "PROGRAM",
  "VAR",

  "FUNCTION",
  "PROCEDURE",

  "INTEGER",
  "REAL",
  "BOOLEAN",

  "IF",
  "THEN",
  "ELSE",
  "WHILE",
  "DO",
  "REPEAT",
  "UNTIL",
  
  "OR",
  "AND",
  "NOT",

  "MOD",
  "DIV",

  "END"

};

int iskeyword(char const *symbol)
{
  int i;
  for(i = BEGIN; i <= END; i++){
    if(strcmp(keywords[i-BEGIN], symbol) == 0) return i;
  }
  return 0;
}


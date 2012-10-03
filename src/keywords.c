#include "keywords.h"

char *keywords[]{
  "BEGIN",
  "PROGRAM",
  "VAR",

  "FUNCTION",
  "PROCEDURE",

  "INTEGER",
  "REAL",

  "END"

};

iskeyword(char const *symbol)
{
  int i;
  for(i = BEGIN; i <= END; i++){
    if(strcmp(keywords[i-BEGIN], symbol) == 0) return i;
  }
}

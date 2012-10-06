#ifndef __KEYWORDS_H__
#define __KEYWORDS_H__

#include <string.h>

enum {
  BEGIN=2048,
  PROGRAM,
  VAR,

  FUNCTION,
  PROCEDURE,

  INTEGER,
  REAL,

  IF,
  THEN,
  ELSE,
  WHILE,
  DO,
  REPEAT,
  UNTIL,

  OR,
  AND,
  NOT,

  MOD,
  DIV,

  END
};

int iskeyword(char const *symbol);

#endif /*__KEYWORDS_H__*/


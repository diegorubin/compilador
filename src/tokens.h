#ifndef __TOKENS_H__
#define __TOKENS_H__

#include "keywords.h"

typedef int token_t;

enum {
  ID=1024,
  ASSGNMT,
  UINT,
  FLOAT,
  GEQ,
  LEQ,
  NEQ,
};

#endif /*__TOKENS_H__*/


#ifndef __TOKENS_H__
#define __TOKENS_H__

typedef int token_t;

enum {
  T_SPACE = ' ',
  T_EOL = '\n',

  T_VAR = 1024,
  T_CONS = 1025,
};

#endif /*__TOKENS_H__*/


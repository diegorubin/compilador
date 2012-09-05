#include <stdio.h>
#include "parser.h"

/** Topdown recursive parser
 * 
 * requires an LL(1) normalized grammar:
 */

/*
 * mybc -> Expr() <enter>
 */
void mybc(void)
{
  Expr();
  match('\n');
}

/*
 * Expr -> [-] Term {plus Term} 
 */
void Expr(void)
{
  int negate = 0;
  /** */if(negate = (lookahead == '-')) match('-');/** */
  Term();
  while(lookahead == '+' || lookahead == '-') {
    match(lookahead); Term();
  }
  /** */{negate && printf(" %c ", '-');}/** */
}

/*
 * Term -> Factor {times Factor}
 */
void Term(void)
{
  Factor();
  while(lookahead == OTIMES){
    match(OTIMES); Factor();
  }
}
/*
 * Factor -> NUM | OCTA | HEXA | ID | FLOAT | (E)
 */
void Factor()
{
  switch (lookahead) {
  case NUM:
  case FLOAT:
  case ZERO:
  case OCTA:
  case HEXA:
  case ID:
    match(lookahead);
    break;
  default:
    match('(');
    Expr();
    match(')');
  }
}


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
  int oplus = 0;

  /** */if(negate = (lookahead == '-')) match('-');/** */
  Term();
  while(lookahead == '+' || lookahead == '-') {
    oplus = lookahead;

    match(lookahead);
    Term();

    /** */{printf(" %c ", oplus);}/** */
  }
  /** */{negate && printf(" %c ", '-');}/** */
}

/*
 * Term -> Factor {times Factor}
 */
void Term(void)
{
  int otimes = 0;

  Factor();
  while(lookahead == OTIMES){
    otimes = lexeme[0];

    match(OTIMES); 
    Factor();

    /** */{printf(" %c ", otimes);}/** */
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
  case OCTA:
  case HEXA:
  case ID:
    match(lookahead);
    /** */{printf(" %s ", lexeme);}/** */
    break;
  default:
    match('(');
    Expr();
    match(')');
  }
}

